/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_delegate.cpp

    \brief  Sertainty delgate management sample program using native C++ language interface.  Uses XML to create ID.

    \author   Greg Smith
    \date     02/17/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"
#include "uxpid.h"
#include "uxpidmanager.h"
#include "uxpservices.h"
#include "uxpdelegateserviceinterface.h"
#include "uxldir.h"

#include <iostream>
#include <string.h>

/**
 * @brief Private external user validation routine.
 * @param username User attempting access.
 * @param data Optional user data.
 * @return True if valid.
 */
static int myValidate(const char *username, void *data)
{
  const char *str = reinterpret_cast<const char *>(data);

  return true;
}

/**
 * @brief Callback function to allow host to evaluate a valid delegate user lookup.
 * @param username User attempting access.
 * @param app_data1 Optional application data.
 * @param app_data2 Optional application data.
 * @param owner Delegate owner.
 * @param delegate Delegate name.
 * @return
 */
static int myDelegateLookup(const char *username,
                            const char *app_data1,
                            const char *app_data2,
                            const char *owner,
                            const char *delegate)
{
  std::cout << "********************************************" << std::endl;
  std::cout << "Delegate lookup callback:" << std::endl << std::endl;
  std::cout << "  Username:       " << username << std::endl;
  std::cout << "  App data1:      " << app_data1 << std::endl;
  std::cout << "  App data2:      " << app_data2 << std::endl;
  std::cout << "  Delegate owner: " << owner << std::endl;
  std::cout << "  Delegate name:  " << delegate << std::endl;
  std::cout << "********************************************" << std::endl;

  return true;
}

/**
 * Response from the user
 * @param ch Challenge object
 * @param prefix Response prefix
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxp::challenge &ch, const std::string &prefix);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  bool licensed;
  std::string outfile("$(SAMPLE)/temp/sample.uxp");
  uxl::bytearray idbuf, outbuf, id, id2;
  int status, loop_cnt;
  bool done;
  uxp_int64 session;
  std::vector<uxl::variant> challenges;
  uxp::delegate_t delegate;
  uxp::user_t user;
  uxp::userid_t userid;
  uxp::subscription_t subscription;
  char identstring[1000];

  std::cout << std::endl << std::endl << "Sample C++ Delegate Application using ID" << std::endl << std::endl;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("SampleDelegateCPP", "Sample Delegate C++ 1.0.0");

    /* Attempt to find the license file. */

    try
    {
      uxp::sys::initializeLibrary(argc, argv,
                                  "sertainty.lic",
                                  "SertintyONE");
      licensed = true;
    }
    catch (uxl::exception &)
    {
      licensed = false;
    }

    /* Initialize Services library */

    uxp::services *lib = uxp::services::getInstance();
    uxp::delegateServiceInterface &dl = *uxp::delegateServiceInterface::getInstance(lib);

    uxp::sys::setLogging(uxpLogInfo);

    if (!licensed)
    {
      std::cout << "UXP-432, A valid license was not found." << std::endl;

      return 1;
    }

    strcpy(identstring, "In validation");

    lib->setValidationCallback(myValidate, identstring);
    dl.setDelegateCallback(myDelegateLookup);

    /* Create a new ID from the sampleid.xml ID definition */

    std::cout << "Setting up required IDs" << std::endl;

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", idbuf);
    
    uxp::idManager::publishToBuffer(id, idbuf);

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid2.xml", idbuf);

    uxp::idManager::publishToBuffer(id2, idbuf);

    /* Create new data services database from ID */

    std::cout << "Opening new services database" << std::endl << std::endl;

#define SAMPLE_MYSQL
#ifndef SAMPLE_MYSQL
    lib->dropDatabase("UXP", {});

    lib->initializeDatabase(id, "SampleUser@myemail.com", "UXP");
#else
    lib->dropDatabase("MySQL", {"so-osx.example.com","3306","uxpds"});

    lib->initializeDatabase(id, "SampleUser@myemail.com", "MySQL", {"so-osx.example.com","3306","uxpds"});

#endif

    /* Open the database ... includes authentication. */

    std::cout << "Credentials necessary to access database:" << std::endl << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;

    /* Open session with no timeout */
    
    session = lib->openSession("SampleUser@myemail.com", 1);

    done = false;

    while (!done)
    {
      status = lib->authenticate(session);

      switch (status)
      {
        case StatusAuthorized:
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
            status = getResponse(t_ch, "Response");
            if (!status)
            {
              std::cout << "Canceled" << std::endl;
              lib->closeSession(session);
              lib->closeDatabase();

              return 1;
            }

            lib->addResponse(session, t_ch);
          }
          break;

        default:
          break;
      }
    }

    lib->setServer(session, "file:///local");

    std::cout << "Creating 2nd user and delegate" << std::endl;

    /* Create a 2nd user.  We already have one when we created the database */

    lib->newUser(session, "SampleUser2@myemail.com",
                "User number 2","Joe User",
                "SampleUser2@myemail.com",
                PRIV_NORMAL,"Sample App Data","More App Data");

    lib->newUserId(session, "SampleUser2@myemail.com",
                  "MyID", 12,
                  "My main ID",
                  id2,
                  uxl::bytearray());

    /* Test */
    
    //std::cout << "Sleep for 600 seconds ... " << std::endl;
    //uxp::sys::sleep(600);
    
    /* Create a delegate */

    dl.newDelegate(session, "SampleUser@myemail.com",
                   "MyList",
                   "This is a test delegate",
                   uxl::datetime(), 1, 24, 0,
                   DELEGATE_FLAGS_PUBLIC);

    /* Create a subscription for user number 2.  This will permit user 2 to connect to UXP
       objects that have the delegate as a user.  When the validation occurs, user 2 will
       be able to log in using the ID that was registered when the user was added to the
       delegate server database.
    */

    dl.subscribe(session,
                 "SampleUser@myemail.com",
                 "MyList",
                 "SampleUser2@myemail.com",
                 uxl::datetime(), 1, 2, 10, 0);

    std::cout << std::endl << "Users" << std::endl;
    std::cout << "-----" << std::endl;

    loop_cnt = 0;

    for (auto const &p: lib->getUsers(session))
    {
      if (loop_cnt++)
        std::cout << std::endl;

      std::cout << "Username: " << p << std::endl;

      lib->getUser(session, p, user.formalname,
                  user.description, user.email,
                  user.privileges,
                  user.app_data1, user.app_data2);

      std::cout << "Formalname: " << user.formalname << std::endl;
      std::cout << "Description: " << user.description << std::endl;
      std::cout << "Email: " << user.email << std::endl;
      std::cout << "Privileges: " << user.privileges << std::endl;
      std::cout << "AppData1: " << user.app_data1 << std::endl;
      std::cout << "AppData2: " << user.app_data2 << std::endl;
    }

    std::cout << std::endl << "Delegates for SampleUser@myemail.com" << std::endl;
    std::cout << "------------------------------------" << std::endl;

    loop_cnt = 0;

    for (auto const &p: dl.getDelegates(session, "SampleUser@myemail.com"))
    {
      if (loop_cnt++)
        std::cout << std::endl;

      dl.getDelegate(session, "SampleUser@myemail.com", p,
                     delegate.description, delegate.expiration,
                     delegate.permit_offline, delegate.offline_duration,
                     delegate.access_max,
                     delegate.flags,
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

    std::cout << std::endl << "Subscribers" << std::endl;
    std::cout << "-----------" << std::endl;

    loop_cnt = 0;

    for (auto const &p: dl.getSubscribers(session, "SampleUser@myemail.com", "MyList"))
    {
      dl.getSubscription(session, "SampleUser@myemail.com",
                         "MyList", p,
                         subscription.expiration,
                         subscription.permit_offline,
                         subscription.offline_duration,
                         subscription.access_max,
                         subscription.access_count);

      if (loop_cnt++)
        std::cout << std::endl;

      std::cout << "Delegate: MyList" << std::endl;
      std::cout << "Subscriber: " << p << std::endl;
      std::cout << "Expiration: " << subscription.expiration.toString() << std::endl;
      std::cout << "Permit Offline: " <<subscription.permit_offline << std::endl;
      std::cout << "Offline Duration: " << subscription.offline_duration << std::endl;
      std::cout << "Access Max: " << subscription.access_max << std::endl;
      std::cout << "Access Count: " << subscription.access_count << std::endl;
    }

    std::cout << std::endl << "Subscriptions for SampleUser2@myemail.com" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    loop_cnt = 0;

    for (auto const &p: dl.getSubscriptions(session, "SampleUser2@myemail.com"))
    {
      if (loop_cnt++)
        std::cout << std::endl;

      std::cout << "Owner: " << p.first << std::endl;
      std::cout << "Name: " << p.second << std::endl;
    }

    /* Create a new UXP document with the delegate ID */

    std::cout << std::endl << "Creating new UXP containing delegate: " <<
                 uxl::fileinfo(outfile).filename() << std::endl;
    std::cout << "Protecting document data.pdf" << std::endl;

    uxp::file data;
    uxp::challenge ch;

    /* We are using the actual delegate ID to protect data.  Though easy to do, this is
     * not recommented as you have little rule control.  Instead, create a new ID
     * by adding any personal user as well as the delegate user from the delegate ID.
     * Then, set the rules for the ID and publish it to an IIC.
     */

    data.openNewFile(outfile, delegate.uxpid, IdBuffer, ModifierReplace, 0);
    data.addVirtualFileFromFile("data.pdf", std::string("$(SAMPLE)/SampleData/data.pdf"), -1, -1, 0);
    data.close();

    std::cout << "Closing new UXP" << std::endl;

    std::cout << "Opening new UXP" << std::endl << std::endl;

    std::cout << "Credentials necessary to access UXP via Delegate Services:" << std::endl << std::endl;
    std::cout << "  Username = SampleUser2@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = MyResponse 1" << std::endl;
    std::cout << "  Challenge 2 = MyResponse 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = MyResponse 10" << std::endl << std::endl;

    data.openFile(outfile, ShareReadOnly);

    done = false;

    while (!done)
    {
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
          for (auto &t_ch: data.getChallenges())
          {
            uxp::challenge ch(t_ch);

            status = getResponse(ch, "MyResponse");
            if (!status)
            {
              std::cout << "Canceled" << std::endl;
              data.close();

              return 1;
            }

            data.addResponse(ch);
          }
          break;

        default:
          break;
      }
    }

    /* Read the virtual file and write it back out to disk */

    std::cout << "Extracting data.pdf to copy.pdf" << std::endl;

    data.exportVirtualFile("data.pdf", "$(SAMPLE)/temp/copy.pdf", ModifierReplace);

    data.close();

    std::cout << std::endl << "Subscribers after Remote Authentication" << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    loop_cnt = 0;

    for (auto const &p: dl.getSubscribers(session, "SampleUser@myemail.com", "MyList"))
    {
      dl.getSubscription(session, "SampleUser@myemail.com",
                         "MyList", p,
                         subscription.expiration,
                         subscription.permit_offline,
                         subscription.offline_duration,
                         subscription.access_max,
                         subscription.access_count);

      if (loop_cnt++)
        std::cout << std::endl;

      std::cout << "Delegate: MyList" << std::endl;
      std::cout << "Subscriber: " << p << std::endl;
      std::cout << "Expiration: " << subscription.expiration.toString() << std::endl;
      std::cout << "Permit Offline: " <<subscription.permit_offline << std::endl;
      std::cout << "Offline Duration: " << subscription.offline_duration << std::endl;
      std::cout << "Access Max: " << subscription.access_max << std::endl;
      std::cout << "Access Count: " << subscription.access_count << std::endl;
    }

    lib->closeDatabase();

    std::cout << std::endl << "Sample finished running" << std::endl;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;
  }

  return 0;
}

/**
 * Response from the user
 * @param ch Challenge object
 * @param prefix Response prefix
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxp::challenge &ch, const std::string &prefix)
{
  std::string value;
  uxl::string prompt;

  prompt = ch.getPrompt();

  ch.startTimer();

  if (prompt == "Username")
  {
    value = "SampleUser2@myemail.com";
  }
  else
  {
    value = prefix + " " + prompt.mid(10);
  }

  ch.endTimer();
  ch.setValue(value);

  return 1;
}
