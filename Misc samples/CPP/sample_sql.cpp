/*! \copyright Sertainty Corporation, 2018.  All Rights Reserved.

    \file     sample_sql.cpp

    \brief  Sertainty sample program using SQL and native C++ language interface.  Uses XML to create ID.

    \author   Greg Smith
    \date     12/13/2019

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"
#include "uxpid.h"
#include "uxpidmanager.h"
#include "uxpsql.h"

#include <iostream>

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
  uxl::bytearray idbuf, outbuf;
  int status;
  bool done;

  std::cout << std::endl << std::endl << "Sample SQL C++ UXP Application using ID" << std::endl << std::endl;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("SampleSqlCPP", "Sample SQL C++ 2.0.0");

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

    uxp::sys::setLogging(uxpLogInfo);

    if (!licensed)
    {
      std::cout << "UXP-432, A valid license was not found." << std::endl;

      return 1;
    }

    uxp::file data;

    /* Create a new ID from the sampleid.xml ID definition */

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", idbuf);

    uxp::idManager::publishToFile("$(SAMPLE)/temp/sampleid.iic", idbuf, ModifierReplace);

    /* Create new UXP from ID */

    std::cout << "Opening new UXP: " << outfile << std::endl;

    data.openNewFile(outfile, "$(SAMPLE)/temp/sampleid.iic", IdFile, ModifierReplace|ModifierReadWrite, 0);

    /* Create a SQL connection in order to use the UXP as a SQL database */

    uxp::sql::query query(&data);

    std::cout << std::endl << "Creating SQL tables" << std::endl;

    query.exec("create table t1 (c1 string, c2 number)");
    query.exec("create index i1 on t1 (c1)");

    std::cout << "Adding data" << std::endl;

    query.exec("insert into t1 (c1,c2) values ('Greg',7)");
    query.exec("insert into t1 (c1,c2) values ('Rose',38)");
    query.exec("insert into t1 (c1,c2) values ('George',12)");

    std::cout << "Testing rollback" << std::endl;

    query.exec("begin transaction");
    query.exec("insert into t1 (c1,c2) values (\"Pete\",10)");
    query.exec("rollback transaction");

    /* Verify 3 rows created */

    query.prepare("select count(*) from t1");
    query.exec();
    query.next();
    query.finalize();

    std::cout << "Rows inserted: " << query.valueInt64(0) << std::endl << std::endl;

    /* Close the UXP */

    data.close();

    /* Reopen the UXP ... includes authentication. */

    std::cout << "Opening new UXP" << std::endl << std::endl;

    std::cout << "Credentials necessary to access UXP:" << std::endl << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;

    data.openFile(outfile, ShareAll);

    done = false;

    while (!done)
    {
      status = data.authenticate();

      switch (status)
      {
        case StatusAuthorized:
          std::cout << "You are authorized" << std::endl;
          done = true;
          break;

        case StatusNotAuthorized:
          std::cout << "You are not authorized" << std::endl;
          return 1;

        case StatusChallenged:
          for (auto &t_ch: data.getChallenges())
          {
            uxp::challenge ch(t_ch);

            status = getResponse(ch, "Response");
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

    std::cout << std::endl << "Fetching all rows using SQL" << std::endl << std::endl;

    uxp::sql::query q2(&data);
    int cnt = 1;

    q2.prepare("select *, rowid from t1");
    q2.exec();

    while (q2.next())
    {
      std::cout << "Row: " << cnt++ << std::endl;

      for (int i = 0; i < q2.columnCount(); i++)
      {
        uxl::variant var = q2.value(i);
        std::string table = q2.columnTable(i);

        std::cout << "  " << table << "." << var.getName() << ": " << var.toString() << std::endl;
      }
    }

    std::cout << std::endl << "Fetching rows using SQL filter: LIKE G%" << std::endl << std::endl;

    q2.prepare("select *,rowid from t1 where c1 like ?");
    q2.bindValue(0, std::string("G%"));
    q2.exec();

    cnt = 1;

    while (q2.next())
    {
      std::cout << "Row: " << cnt++ << std::endl;

      for (int i = 0; i < q2.columnCount(); i++)
      {
        uxl::variant var = q2.value(i);
        std::string table = q2.columnTable(i);

        std::cout << "  " << table << "." << var.getName() << ": " << var.toString() << std::endl;
      }
    }

    q2.finalize();
    data.close();

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
    value = "SampleUser@myemail.com";
  }
  else
  {
    value = prefix + " " + prompt.mid(10);
  }

  ch.endTimer();
  ch.setValue(value);

  return 1;
}
