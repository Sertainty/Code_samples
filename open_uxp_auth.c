/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     open_uxp_auth.c

    \brief    This sample shows how to open and interactively authenticate a UXP Object
              using known prompt/response pairs and read contents of virtual files protected
              within a UXP Object.

    \author   Karim Lalani
    \date     06/15/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile_c.h"
#include "uxpfile_common.h"
#include "uxplist_c.h"
#include "uxlbytearray_c.h"
#include "uxpvfile_c.h"
#include "uxpcredential_c.h"
#include "uxpid_c.h"
#include "uxpsys_c.h"

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

/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @return True if accepted.  False if cancelled
 */
static int getResponse(uxpChallengeHandle ch);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpFileHandle appHandle = NULL;
  uxpOpenVFHandle fileHandle;
  FILE *fp;
  long long len;
  int status;
  int done,i;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";

  printf("\nOpening and Authenticating into a UXP interactively\n");

  /* Allocate a new buffer. Automatically zeros it. */

  buffer = uxpba_newHandle();

  /*
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleC", "Sample 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  printf("\nOpening new Data\n");

  appHandle = uxpfile_newHandle();
  uxpfile_openFile(appHandle, uxpFileSpec, ShareReadOnly);
  printf("File Open.\nChecking for Errors.\n");
  checkError(uxpfile_errorStatus(appHandle), "Opening UXP");

  printf("\nCredentials necessary to access UXP:\n");
  printf("  Username = SampleUser@myemail.com\n");
  printf("  Challenge 1 = Response 1\n");
  printf("  Challenge 2 = Response 2\n");
  printf("  ... \n");
  printf("  Challenge 10 = Response 10\n\n");

  done = 0;

  /* Continue to authenticate until a final decision is reached */

  while (!done)
  {
    status = uxpfile_authenticate(appHandle, 0);

    switch (status)
    {
      case StatusAuthorized:
        printf(" - You are authorized\n");
        done = 1;
        break;

      case StatusNotAuthorized:
        printf(" - You are not authorized\n");
        return 1;

      case StatusChallenged:
        for (i = 0; i < uxpfile_getChallengeCount(appHandle); i++)
        {
          uxpChallengeHandle ch = uxpfile_getChallenge(appHandle, i);

          getResponse(ch);

          uxpfile_addResponse(appHandle, ch);

          uxpch_freeHandle(ch);
        }
        break;

      default:
        break;
    }
  }

  printf("\nExtracting data.pdf to copy2.pdf\n");

  fileHandle = uxpfile_openVirtualFile(appHandle, "data.pdf", ModeRead);
  checkError(uxpfile_errorStatus(appHandle), "Opening virtual file");

  uxpba_setData(buffer, copy2Spec, strlen(copy2Spec));
  uxpsys_processStandardMacros(buffer);

  fp = fopen(uxpba_getData(buffer), "wb");

  while (uxpfile_readVirtualFile(appHandle, fileHandle, buffer, 100000))
  {
    len = uxpba_getSize(buffer);
    fwrite(uxpba_getData(buffer), len, 1, fp);
  }

  fclose(fp);

  uxpfile_closeVirtualFile(appHandle, fileHandle);

  if (uxpfile_compareExternalFile(appHandle, "data.pdf", copy2Spec))
    printf("Comparison of data.pdf to copy2.pdf: successful\n");
  else
    printf("Comparison of data.pdf to copy2.pdf: failed\n");

  /* Close up again and free handle */

  uxpfile_close(appHandle);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpfile_freeHandle(appHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("You have successfully opened and interactively authenticated into a UXP.\n");
  printf("You may try out other advanced samples now.\n");

  printf("\nSample finished running\n");

  return 0;
}

/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @return True if accepted.  False if cancelled
 */
static int getResponse(uxpChallengeHandle ch)
{
  char value[1000];
  uxlByteArray *prompt = uxpba_newHandle();

  uxpch_getPrompt(ch, prompt);
  uxpch_startTimer(ch);

  if (!strcmp(uxpba_getData(prompt), "Username"))
  {
    uxpch_setValueString(ch, "SampleUser@myemail.com");
  }
  else
  {
    strcpy(value, "Response ");

    strcat(value, uxpba_getData(prompt) + 10);
    uxpch_setValueString(ch, value);
  }

  uxpch_endTimer(ch);
  uxpba_freeHandle(prompt);

  return 1;
}
