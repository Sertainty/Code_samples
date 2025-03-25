/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     delegate_benchmark_setup.cpp

    \brief    Add a new user to DS Database.
              Adds delegates to the database and users to those delegates

    \author   Quinn Carter
    \date     10/7/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"
#include "uxpid.h"
#include "uxpservices.h"
#include "uxpdelegateserviceinterface.h"
#include "uxpsys.h"
#include "uxpidmanager.h"
#include "uxlfileinfo.h"
#include "unistd.h"
#include "sys/types.h"
#include "pwd.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <ctime>
#include <string>

#if defined(__clang_major__) && (defined(LINUX) || defined(MACOSX))
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wunknown-pragmas"

#if __clang_major__ >= 6
#pragma clang diagnostic ignored "-Wunused-lambda-capture"
#endif

#include "boost/program_options.hpp"
#pragma clang diagnostic pop
#else
#include "boost/program_options.hpp"
#endif

namespace po = boost::program_options;

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  try
  {
    uxl::bytearray buffer, idbuf, id, id2, outbuf;
    bool licensed;
    bool new_user = false;
    bool existing_user = false;
    std::string dbFileSpec("$(SAMPLE)/temp/dataservices.db");
    uxp_int64 session;
    uxp::delegate_t delegate;
    uxp::subscription_t subscription;
    std::string outfile("$(SAMPLE)/temp/delegateBenchmark.uxp");
    int userXmlCount;
    std::string delegateName = "TestDelegate";
    int status;
    std::string newDB;
    std::string option = "UXP";
    std::string homePath = getenv("HOME");

    std::string host, pwd;
    std::string port;

    uintmax_t finalSize;
    uintmax_t initSize;
    uintmax_t dataSize;
    uintmax_t uxpSize;
    uintmax_t uxpOverheadSize;

    time_t curr_time;
    tm * curr_tm;
    char date_string[100];
    time(&curr_time);
    curr_tm = localtime(&curr_time);
    strftime(date_string, 100, "%B-%d-%Y-%I-%M-%S", curr_tm);

    std::cout << "Delegate benchmark" << std::endl << std::endl;

    /* Process command line args */

    po::options_description desc("Sertainty Delegate Benchmark");

    desc.add_options()
        ("help,h", "Produce help message")
        ("database,d", po::value<std::string>(), "DataServices database option.  Possible values: UXP, MySQL")
        ("count,c", po::value<int>(), "Specifies the number of users to add to benchmark.")
        ("url,u", po::value<std::string>(), "Specifies the database host url.  MySQL only.")
        ("port,p", po::value<int>(), "Specifies the database port.  MySQL only.")
        ("authentication,a", po::value<std::string>(), "Specifies the database password.  MySQL only.")
    ;

    po::variables_map vm;

    try
    {
      po::store(po::command_line_parser(argc, argv).
                options(desc).run(), vm);
      po::notify(vm);
    }
    catch (std::exception &e)
    {
      std::cout << "Delegate benchmark error: " << e.what() << std::endl << std::endl;
      std::cout << desc << std::endl;

      exit (EXIT_FAILURE);
    }

    if (vm.count("help"))
    {
      std::cout << desc << std::endl;

      exit (EXIT_SUCCESS);
    }

    if (vm.count("count"))
    {
      userXmlCount = vm["count"].as<int>();
    }

    if (vm.count("database"))
    {
      option = vm["database"].as<std::string>();
      if (option != "UXP" && option != "MySQL")
      {
        std::cout << "Invalid database: " << option << std::endl << std::endl;
        std::cout << desc << std::endl;

        exit (EXIT_FAILURE);
      }
    }

    if (vm.count("url"))
    {
      host = vm["url"].as<std::string>();
    }

    if (vm.count("authentication"))
    {
      pwd = vm["authentication"].as<std::string>();
    }

    if (vm.count("port"))
    {
      port = uxl::string::number(vm["port"].as<int>());
    }

    if ((option == "MYSQL") && (host.empty() || port.empty() || pwd.empty()))
    {
      std::cout << "MySQL requires host/port/authentication" << std::endl << std::endl;
      std::cout << desc << std::endl;

      exit (EXIT_FAILURE);
    }

    std::cout << "Benchmark database: " << option << " option" << std::endl << std::endl;

    /*
      Enter in platform in the file name
    */
    std::string date;

    std::string txtFileName = homePath + "/Sertainty/developer/examples/temp/DelegateBenchmark_";
    txtFileName.append(date_string);
    txtFileName.append(".txt");
    std::ofstream txtfile;
    //txtFileName = "$(SAMPLE)/temp/" + txtFileName;
    txtfile.open(txtFileName);

    std::string createUser_fn = homePath + "/Sertainty/developer/examples/temp/CreateUsers_";
    createUser_fn.append(date_string);
    createUser_fn.append(".txt");
    std::ofstream createfile;
    createfile.open(createUser_fn);

    std::string subscribeUser_fn = homePath + "/Sertainty/developer/examples/temp/Subscribe_Users_";
    subscribeUser_fn.append(date_string);
    subscribeUser_fn.append(".txt");
    std::ofstream subscribefile;
    subscribefile.open(subscribeUser_fn);

    std::string newUserID_fn = homePath + "/Sertainty/developer/examples/temp/New_UserID_";
    newUserID_fn.append(date_string);
    newUserID_fn.append(".txt");
    std::ofstream newUserID_file;
    newUserID_file.open(newUserID_fn);
    
    std::string subscribe_userExists_fn = homePath + "/Sertainty/developer/examples/temp/Subscribe_UserExists_";
    subscribe_userExists_fn.append(date_string);
    subscribe_userExists_fn.append(".txt");
    std::ofstream subscribe_userExist_file;
    subscribe_userExist_file.open(subscribe_userExists_fn);

    /* Set up log file and initialize library.  Must do this before any active calls. */

    try
    {
      uxp::sys::initializeLibrary(argc, argv,"sertainty.lic","SertintyONE");
      licensed = true;
    }
    catch (uxl::exception &)
    {
      licensed = false;
    }

    uxp::sys::setLogging(uxpLogInfo);

    if (!licensed)
    {
      std::cout << "Error Error initializing environment" << std::endl;
      std::cout << "Sample failed. Ensure you have a valid installation and license." << std::endl;
      
      return 1;
    }

    uxp::services *lib = uxp::services::getInstance();
    uxp::delegateServiceInterface &dl = *uxp::delegateServiceInterface::getInstance(*lib);

    /* Create a new ID from the sampleid.xml ID definition */

    std::cout << "Setting up required IDs" << std::endl;

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", idbuf);

    uxp::idManager::publishToBuffer(id, idbuf, ModifierIdentityV2 | ModifierCompress);

    bool select_option = false;

    /* Create new database or use existing database */

    while (!select_option)
    {
      std::cout << "Would you like to create a new database? (Y/N): ";
      std::cin >> newDB;

      if (newDB == "Y" || newDB == "y")
      {
        std::cout << "Dropping existing services database" << std::endl;

        lib->dropDatabase(option, {host, port, pwd});
        //lib->dropDatabase("MySQL", {"so-osx.example.com","3306","uxpds"});

        std::cout << "Initializing new data services database" << std::endl;

        lib->initializeDatabase(id, "SampleUser@myemail.com", option, {host, port, pwd});
        //lib->initializeDatabase(id, "SampleUser@myemail.com", "MySQL", {"so-osx.example.com","3306","uxpds"});

        select_option = true;
      }
      else if (newDB == "N" || newDB == "n")
      {
        std::cout << "Opening existing data services database" << std::endl;
        select_option = true;
      }
      else
      {
        std::cout << "Invalid input!" << std::endl;
      }
    }

    /* If database exists already use this code below */

    lib->openDatabase(0);

    std::cout << "Opening Database ... " << std::endl;

    std::cout << "Size of database after opening: " << std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes"<<std::endl;  //for tracking file size of the db after opening
    initSize = std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db");
    txtfile << "Size of database after opening: " << std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << std::endl;

    session = lib->openSession("SampleUser@myemail.com");

    /* Authenticate into the DS DB with a known username.
    * The username belongs to an ID already present in DB
    * and the username is provided in the uxpds_openSession call earlier.
    * The ID must have SESSION privileges in order to be used to open session on DS DB. */

    bool done = false;

    while (!done)
    {
      int status = lib->authenticate(session);

      switch (status)
      {
        case StatusAuthorized:
          std::cout << "You are authorized" << std::endl;
          done = true;
          break;

        case StatusNotAuthorized:
          std::cout << "You are not authorized" << std::endl;
          lib->closeSession(session);
          lib->closeDatabase();
          return 1;

        case StatusChallenged:
          for (auto t_ch: lib->getChallenges(session))
          {
            uxl::string v = t_ch.getPrompt();
            std::string num = v.mid(10);

            t_ch.setValue("Response " + num);

            lib->addResponse(session, t_ch);
          }
          break;

        default:
          break;
      }
    }

    lib->setServer(session, "file:///local");

    /* loop user for user id creation from xmls */
    
    if (!userXmlCount)
    {
      std::cout << "How many users to be entered into the database?: ";
      std::cin >> userXmlCount;
      delegateName = delegateName + "_" + std::to_string(userXmlCount);
    }
    else
    {
      delegateName = delegateName + "_" + std::to_string(userXmlCount);
    }

    int createUserL[userXmlCount];
    int subTimeL[userXmlCount];
    int newUserID_TimeL[userXmlCount];
    int sub_userExistL[userXmlCount];

    txtfile << std::endl << userXmlCount << " User(s) are being added to the database" << std::endl;

    if (!dl.delegateExists(session,"SampleUser@myemail.com",delegateName))
    {
      dl.newDelegate(session, "SampleUser@myemail.com",
                      delegateName,
                      "This is a test delegate",
                      uxl::datetime(), 1, 24, 0,
                      DELEGATE_FLAGS_PUBLIC);

      std::cout << "Creating the " << delegateName << " delegate ..." << std::endl;
    }
    else
    {
      std::cout << "This Delegate already exists. Exiting...";
      return 1;
    }

    std::cout << "Size of database after creating the "<<  delegateName << " delegate: " << std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes"<<std::endl;
    txtfile << std::endl << "Size of database after creating the "<<  delegateName << " delegate: " << std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes"<< std::endl;
    
    std::cout << std::endl;

    auto start_WholeTime = std::chrono::high_resolution_clock::now();
    for (int x = 1; x <= userXmlCount; x++)
    {
      int y = x-1;

      std::string userEmail = uxl::string::format("SampleUser%1@myemail.com", x);

      std::cout << "Creating user and subscription:" << userEmail << std::endl;

      uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid2.xml", buffer);
      buffer.replace("SampleUser2@myemail.com", userEmail.c_str());
      uxp::idManager::publishToBuffer(id2, buffer, ModifierIdentityV2 | ModifierCompress);

      if (!lib->userExists(session, userEmail))
      {
        new_user = true;
        try
        {
          auto start_createUser = std::chrono::high_resolution_clock::now();
          lib->newUser(session, userEmail,
                      userEmail,"Joe Sample",
                      userEmail,
                      PRIV_NORMAL,"Sample App Data","More App Data");
          auto end_createUser = std::chrono::high_resolution_clock::now();
          auto createUser_time = std::chrono::duration_cast<std::chrono::microseconds>(end_createUser - start_createUser);
          int createUser_timeCount = createUser_time.count();
          createUserL[y] = createUser_timeCount;
          createfile << userEmail << ": " << createUser_timeCount << std::endl;
        }
        catch (uxl::exception &e2)
        {
          std::cout << "Error: " << e2.getErrorText() << std::endl;
          std::cout << "PROBLEM lib->newUser" << std::endl;

          return 1;
        }
        
        try
        {
          auto start_newUserID = std::chrono::high_resolution_clock::now();
          lib->newUserId(session, userEmail,
                        "MyID", 12,
                        "My main ID",
                        id2,
                        uxl::bytearray());
          auto end_newUserID = std::chrono::high_resolution_clock::now();
          auto newUserID_time = std::chrono::duration_cast<std::chrono::microseconds>(end_newUserID - start_newUserID);
          int newUserID_timeCount = newUserID_time.count();
          newUserID_TimeL[y] = newUserID_timeCount;
          newUserID_file << userEmail << ": " << newUserID_timeCount << std::endl;
        }
        catch (uxl::exception &e2)
        {
          std::cout << "Error: " << e2.getErrorText() << std::endl;
          std::cout << "PROBLEM lib->newUserId()" << std::endl;

          return 1;
        }

        buffer.clear();
        id2.clear();

        try
        {
          auto start_subUser = std::chrono::high_resolution_clock::now();
          dl.subscribe(session,
                        "SampleUser@myemail.com",
                        delegateName,
                        userEmail,
                        uxl::datetime(), 1, 2, 10, 0);
          auto end_subUser = std::chrono::high_resolution_clock::now();
          auto subUser_time = std::chrono::duration_cast<std::chrono::microseconds>(end_subUser - start_subUser);
          int subUser_timeCount = subUser_time.count();
          subTimeL[y] = subUser_timeCount;
          subscribefile << userEmail << ": " << subUser_timeCount << std::endl;

          std::cout << "Database Size after adding " + userEmail + ": "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes" << std::endl << std::endl;   //for tracking file size of the db
          txtfile << std::endl << "Database Size after adding " + userEmail + ": "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes" << std::endl;
        }
        catch (uxl::exception &e2)
        {
          std::cout << "Error: " << e2.getErrorText() << std::endl;
          std::cout << "PROBLEM lib->subscribe()" << std::endl;

          return 1;
        }
      }
      // If the User exists, subscribe existing user to the new delegate
      else
      {
        existing_user = true;
        std::cout << "User Exists: " + userEmail << std::endl;
        std::cout << "Subcribing User to " + delegateName +" delegate" << std::endl;
        try
        {
          auto start_subUser_exist = std::chrono::high_resolution_clock::now();
          dl.subscribe(session,
                        "SampleUser@myemail.com",
                        delegateName,
                        userEmail,
                        uxl::datetime(), 1, 2, 10, 0);
          auto end_subUser_exist = std::chrono::high_resolution_clock::now();
          auto subUser_exist_time = std::chrono::duration_cast<std::chrono::microseconds>(end_subUser_exist - start_subUser_exist);
          int subUser_exist_timeCount = subUser_exist_time.count();
          sub_userExistL[y] = subUser_exist_timeCount;
          subscribe_userExist_file << userEmail << ": " << subUser_exist_timeCount << std::endl;

          std::cout << "Database Size after subscribing " + userEmail + ": "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes" << std::endl << std::endl;   //for tracking file size of the db
          txtfile << std::endl << "Database Size after subscribing " + userEmail + ": "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") << " bytes" << std::endl;
        }
        catch (uxl::exception &e2)
        {
          std::cout << "Error: " << e2.getErrorText() << std::endl;
          std::cout << "PROBLEM lib->subscribe()" << std::endl;

          return 1;
        }
      }
    }
    auto end_WholeTime = std::chrono::high_resolution_clock::now();
    auto WholeTime_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_WholeTime - start_WholeTime);
    int WholeTime_count = WholeTime_duration.count();
    std::cout << "The time it took from start and end for adding " << userXmlCount << " users: " << WholeTime_count << std::endl << std::endl;

    //Get the average of both the ID Creation Time and ID Subscribe Time
    int sum_CreateTime = 0;
    int sum_SubTime = 0;
    int sum_newUserID_Time = 0;
    int sum_SubExistTime = 0;
    for(int i = 0; i < userXmlCount; i++) 
    {
      if (new_user)
      {
        sum_CreateTime += createUserL[i];
        sum_newUserID_Time += newUserID_TimeL[i];
        sum_SubTime += subTimeL[i];
      }
      else if (existing_user)
      {
        sum_SubExistTime += sub_userExistL[i];
      }
      else
      {
        std::cout << "Error Getting Average!" << sub_userExistL[i] << "\n";
      }
    }
    
    int avg_createTime = sum_CreateTime / userXmlCount;
    int avg_newUserID_Time = sum_newUserID_Time / userXmlCount;
    int avg_subTime = sum_SubTime / userXmlCount;
    int avg_sub_existTime = sum_SubExistTime / userXmlCount;

    std::cout << "Average Creation Time: "<< avg_createTime << std::endl;
    txtfile << std::endl << "Average Creation Time: " << avg_createTime << std::endl;
    createfile << "Average Creation Time: " << avg_createTime << std::endl;
    createfile.close();

    std::cout << "Average New User ID Time: "<< avg_newUserID_Time << std::endl;
    txtfile << std::endl << "Average New User ID Time: " << avg_newUserID_Time << std::endl;
    newUserID_file << "Average Creation Time: " << avg_newUserID_Time << std::endl;
    newUserID_file.close();

    std::cout << "Average Subscribe Time: "<< avg_subTime << std::endl;
    txtfile << std::endl << "Average Subscribe Time: " << avg_subTime << std::endl;
    subscribefile << "Average Subscribe Time: " << avg_subTime << std::endl;
    subscribefile.close();
    
    std::cout << "Average Subscribe Existing User Time: "<< avg_sub_existTime << std::endl;
    txtfile << std::endl << "Average Subscribe Existing User  Time: " << avg_sub_existTime << std::endl;
    subscribe_userExist_file << "Average Subscribe Existing User Time: " << avg_sub_existTime << std::endl;
    subscribe_userExist_file.close();
    
    finalSize = std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/dataservices.db") - initSize;
    
    std::cout << std::endl << userXmlCount << " users added: "<< finalSize << " bytes to the database" << std::endl;
    txtfile << std::endl << userXmlCount << " users added: "<< finalSize << " bytes to the database" << std::endl;

    for (auto const &p: dl.getDelegates(session, "SampleUser@myemail.com"))
    {
      std::cout << std::endl;

      dl.getDelegate(session, "SampleUser@myemail.com", p,
                      delegate.description, delegate.expiration,
                      delegate.permit_offline, delegate.offline_duration,
                      delegate.access_max, delegate.flags,
                      delegate.checksum, delegate.uxpid);

      std::cout << "Name: " << p << std::endl;
      std::cout << "Description: " << delegate.description << std::endl;
      std::cout << "Expiration: " << delegate.expiration.toString() << std::endl;
      std::cout << "Permit Offline: " << delegate.permit_offline << std::endl;
      std::cout << "Offline Duration: " << delegate.offline_duration << std::endl;
      std::cout << "Access Max: " << delegate.access_max << std::endl;
      std::cout << "Checksum: " << delegate.checksum << std::endl;
      std::cout << "Uxpid: " << "******" << std::endl;
    }

    std::cout << std::endl;

    dl.getDelegate(session, "SampleUser@myemail.com", delegateName,
                    delegate.description, delegate.expiration,
                    delegate.permit_offline, delegate.offline_duration,
                    delegate.access_max, delegate.flags,
                    delegate.checksum, delegate.uxpid);

    std::string delegateIdName = homePath + "/Sertainty/developer/examples/temp/" + delegateName + ".iic";
    std::cout << "Writing " + delegateIdName << std::endl;

    uxp::sys::fileWriteAll(delegateIdName,delegate.uxpid,ModifierReplace);

    /*Opening a UXP using the Delegate ID*/

    uxp::file data;
    uxp::challenge ch;
    const char *delegateID = delegateIdName.data();

    std::cout << "Delegate ID size: "<< std::__fs::filesystem::file_size(delegateID) << " bytes"<< std::endl;
    txtfile << std::endl << "Delegate ID size: "<<std::__fs::filesystem::file_size(delegateID) << " bytes"<< std::endl;

    std::cout << "Data to be protected size: "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/SampleData/data.pdf") << " bytes"<< std::endl;
    txtfile << std::endl << "Data to be protected size: "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/SampleData/data.pdf") << " bytes"<< std::endl;
    dataSize = std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/SampleData/data.pdf");

    std::cout << std::endl << "Creating new UXP containing delegate: " << outfile << std::endl;
    std::cout << "Protecting document data.pdf" << std::endl;

    data.openNewFile(outfile, delegateID, IdFile, ModifierReplace, 0);
    data.addVirtualFileFromFile("data.pdf", std::string("$(SAMPLE)/SampleData/data.pdf"), -1, -1, 0);
    data.close();

    std::cout << "Closing new UXP" << std::endl;

    std::cout << "Uxp size: "<< std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/delegateBenchmark.uxp") << " bytes"<< std::endl;
    txtfile << std::endl << "Uxp size: "<<std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/delegateBenchmark.uxp") << " bytes"<< std::endl;
    uxpSize = std::__fs::filesystem::file_size(homePath + "/Sertainty/developer/examples/temp/delegateBenchmark.uxp");
    
    uxpOverheadSize = (uxpSize-dataSize);
    std::cout << "UXP overhead size: "<< uxpOverheadSize << " bytes"<< std::endl;
    txtfile << std::endl << "Uxp overhead size: "<< uxpOverheadSize << " bytes"<< std::endl;

    std::cout << "Opening new UXP" << std::endl << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    data.openFile(outfile, ShareReadOnly);

    done = false;

    auto authTime = std::chrono::high_resolution_clock::now();
    
    while (!done)
    {
      auto authenticateStartTime = std::chrono::high_resolution_clock::now();
      auto authenticateEndTime = std::chrono::high_resolution_clock::now();
      
      status = data.authenticate(false);

      switch (status)
      {
        case StatusAuthorized:
          std::cout << std::endl << "You are authorized via Delegate Services" << std::endl;
          done = true;
          break;

        case StatusNotAuthorized:
          std::cout << std::endl << "You are not authorized" << std::endl;
          return 1;

        case StatusChallenged:
          authenticateEndTime = std::chrono::high_resolution_clock::now();

          for (auto t_ch: data.getChallenges())
          {
            uxl::string v = t_ch.getPrompt();

            if (v == "Username")
              t_ch.setValue("SampleUser2@myemail.com");
            else
            {
              std::string num = v.mid(10);

              t_ch.setValue("MyResponse " + num);
            }

            data.addResponse(t_ch);
          }
          break;

        default:
          break;
      }

      auto challengeTime = std::chrono::duration_cast<std::chrono::microseconds>(authenticateEndTime - authenticateStartTime);
      std::cout << std::endl <<  "Time to get back Challenges/Responses from DB is: " << challengeTime.count() << " microseconds" << std::endl;
      txtfile << std::endl << "Time to get back Challenges/Responses from DB is: " << challengeTime.count() << " microseconds" << std::endl;
    }

    auto authend = std::chrono::high_resolution_clock::now();

    /* Read the virtual file and write it back out to disk */

    std::cout << std::endl << "Extracting data.pdf to copy2.pdf" << std::endl;

    data.exportVirtualFile("data.pdf", "$(SAMPLE)/SampleData/copy.pdf", ModifierReplace);

    data.close();

    auto stop = std::chrono::high_resolution_clock::now();
    //auto authDuration = std::chrono::duration_cast<std::chrono::milliseconds>(authend - authTime);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>((stop - start) - (authend - authTime));
    std::cout << std::endl <<  "Time to open UXP using Delegate ID is: " << duration.count() << " milliseconds" << std::endl;
    txtfile << std::endl << "Time to open UXP using Delegate ID is: " << duration.count() << " milliseconds" << std::endl;
    txtfile.close();

    lib->closeDatabase();

    return 0;
  }
  catch (std::runtime_error &r2)
  {
    std::cout << "Error: " << r2.what() << std::endl;

    try
    {
      return 1;
    }
    catch (std::runtime_error &r2)
    {
    }
  }
}
