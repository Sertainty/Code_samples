/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     uxp_sql.c

    \brief    Sample workflow using UXP as a SQL database within a UXP Object.  Uses XML to create ID.

    \details  Demonstrates UXP SQL – using C  language interface
                (1) Creates ID from xml
                (2) Creates UXP from ID
                (3) Creates SQL tables within UXP object
                (4) Close / Re-opens UXP
                (5) Accesses SQL / data
 
    \author   Greg Smith
    \date     06/20/2018
 
    \edited          Melani Smith Weed /  27-jul-2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile_c.h"
#include "uxpid_c.h"
#include "uxpfile_common.h"
#include "uxpcredential_c.h"
#include "uxlbytearray_c.h"
#include "uxpsys_c.h"
#include "uxpsql_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Checks for an error and logs message
 * @param handle Handle to check.
 * @param msg Message to display.
 */
static void checkError(uxpCallStatusHandle handle, const char *msg)
{
  if (uxpsys_hasError(handle))
  {
    const char *errMsg = uxpsys_getErrorMessage(handle);
    printf("%s error: %s\n", msg, errMsg);
    exit(1);
  }
}

void sqlError(void *e_handle)
{
  if (uxpsys_hasError(e_handle))
  {
    const char *errMsg = uxpsys_getErrorMessage(e_handle);
    printf("SQL error: %s\n", errMsg);
    exit(1);
  }
}

/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @param prefix Response prefix
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxpChallengeHandle ch, const char *prefix);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpFileHandle appHandle;
  int status;
  int done,i;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sampleid.xml";
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample_sql.uxp";

  printf("\n\nSample C Data Application using SQL\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();

  /* Set up log file and initialize library.  Must do this before any active calls. */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleSqlC", "Sample SQL C 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  /* Create a new ID from the sampleid.xml ID definition */

  uxpsys_fileReadAll(callstatus, idXmlSpec, buffer);
  checkError(callstatus, "Reading sample ID XML");

  uxpid_publishToFile(callstatus,
                      idFileSpec,
                      uxpba_getData(buffer),
                      ModifierReplace);
  checkError(callstatus, "Creating ID");

  /* Create new UXP from ID */

  appHandle = uxpfile_newHandle();

  uxpfile_openNewFile(appHandle, uxpFileSpec, idFileSpec, IdFile,
                      ModifierReplace|ModifierReadWrite, 0);
  checkError(uxpfile_errorStatus(appHandle), "Creating new UXP");

  /* Set up queries in order to use the UXP as a SQL database */

  printf("\nCreating SQL tables\n");

  uxpSqlQueryHandle query = uxpsql_newQueryHandle(appHandle);
  sqlError(appHandle);

  uxpsql_execInLine(query, "create table t1 (c1 string, c2 number)");
  sqlError(query);

  uxpsql_execInLine(query, "create index i1 on t1 (c1)");
  sqlError(query);

  printf("Adding data\n");

  uxpsql_execInLine(query, "insert into t1 (c1,c2) values ('Greg',7)");
  sqlError(query);

  uxpsql_execInLine(query, "insert into t1 (c1,c2) values ('Rose',38)");
  sqlError(query);

  uxpsql_execInLine(query, "insert into t1 (c1,c2) values ('George',12)");
  sqlError(query);

  printf("Testing rollback\n");

  uxpsql_execInLine(query, "begin transaction");
  sqlError(query);
  uxpsql_execInLine(query, "insert into t1 (c1,c2) values (\"Pete\",10)");
  sqlError(query);
  uxpsql_execInLine(query, "rollback transaction");
  sqlError(query);

  /* Verify 3 rows created */

  uxpsql_prepare(query, "select count(*) from t1");
  sqlError(query);

  uxpsql_exec(query);
  sqlError(query);

  uxpsql_next(query);
  sqlError(query);

  printf("Rows inserted: %lld\n\n", uxpsql_valueInt64(query, 0));

  /* Close the UXP.  This will delete the handle as well. */

  uxpsql_freeQueryHandle(query);
  uxpfile_close(appHandle);

  /* Reopen the Data ... includes authentication. */

  printf("Opening new Data\n\n");

  uxpfile_openFile(appHandle, uxpFileSpec, ShareReadOnly);
  checkError(uxpfile_errorStatus(appHandle), "Opening new UXP");

  printf("Credentials necessary to access UXP:\n");
  printf("  Username = SampleUser@myemail.com\n");
  printf("  Challenge 1 = Response 1\n");
  printf("  Challenge 2 = Response 2\n");
  printf("  ... \n");
  printf("  Challenge 10 = Response 10\n\n");

  done = 0;

  /* Continue to authenticate until a final decision is reached. */

  while (!done)
  {
    status = uxpfile_authenticate(appHandle, 0);

    switch (status)
    {
      case StatusAuthorized:
        printf("You're authorized\n");
        done = 1;
        break;

      case StatusNotAuthorized:
        printf("You're not authorized\n");
        return 1;

      case StatusChallenged:
        for (i = 0; i < uxpfile_getChallengeCount(appHandle); i++)
        {
          uxpChallengeHandle ch = uxpfile_getChallenge(appHandle, i);

          getResponse(ch, "Response");

          uxpfile_addResponse(appHandle, ch);

          uxpch_freeHandle(ch);
        }
        break;

      default:
        break;
    }
  }

  printf("\nFetching all rows using SQL\n\n");

  query = uxpsql_newQueryHandle(appHandle);
  sqlError(appHandle);

  int cnt = 1;
  uxlByteArray *buf = uxpba_newHandle();
  uxlByteArray *buf2 = uxpba_newHandle();
  uxlByteArray *buf3 = uxpba_newHandle();

  uxpsql_prepare(query, "select *,rowid from t1");
  sqlError(query);
  uxpsql_exec(query);
  sqlError(query);

  while (uxpsql_next(query))
  {
    printf("Row: %d\n", cnt++);

    for (i = 0; i < uxpsql_columnCount(query); i++)
    {
      uxlVariantHandle var = uxpsql_value(query, i);

      uxpsql_columnTable(query, i, buf);

      printf("  %s.%s: %s\n", uxpba_getData(buf), uxpvar_getName(var, buf2), uxpvar_toString(var, buf3));

      uxpvar_freeHandle(var);
    }
  }

  printf("\nFetching rows using SQL filter: LIKE G%%\n\n");

  uxpsql_prepare(query, "select *,rowid from t1 where c1 like ?");
  sqlError(query);
  uxpsql_bindValueText(query, 0, "G%");
  uxpsql_exec(query);
  sqlError(query);

  cnt = 1;

  while (uxpsql_next(query))
  {
    printf("Row: %d\n", cnt++);

    for (i = 0; i < uxpsql_columnCount(query); i++)
    {
      uxlVariantHandle var = uxpsql_value(query, i);

      uxpsql_columnTable(query, i, buf);

      printf("  %s.%s: %s\n", uxpba_getData(buf), uxpvar_getName(var, buf2), uxpvar_toString(var, buf3));

      uxpvar_freeHandle(var);
    }
  }

  /* Close up again and free handle */

  uxpfile_close(appHandle);
  uxpsql_freeQueryHandle(query);
  uxpba_freeHandle(buf);
  uxpba_freeHandle(buf2);
  uxpba_freeHandle(buf3);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpfile_freeHandle(appHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("\nSample finished running\n");

  return 0;
}
/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @param prefix Response prefix for batch mode
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxpChallengeHandle ch, const char *prefix)
{
  char value[1000];
  uxlByteArray *prompt = uxpba_newHandle();

  uxpch_getPrompt(ch, prompt);
  uxpch_startTimer(ch);

  const char *ptr2 = uxpba_getData(prompt);
  char tmp[10];

  if (!strcmp(ptr2, "Username"))
  {
    strcpy(value,"SampleUser@myemail.com");
  }
  else
  {
    strncpy(tmp, ptr2 + 10, 2);
    sprintf(value, "%s %s", prefix, tmp);
  }

  uxpch_endTimer(ch);
  uxpch_setValueString(ch, value);

  uxpba_freeHandle(prompt);

  return 1;
}
