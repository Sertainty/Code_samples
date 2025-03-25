/*!
    \copyright Sertainty Corporation, 2018.  All Rights Reserved.

    \file    id_session.c

    \brief   This sample demonstrates how to authenticate into an ID session
             and use it to auto-authenticate, also known as, Single Sign-On, into a UXP Object.

    \details A UXP Object requires authentication of the prospective user
             (process or person). ßAccess will only be granted after a successful authentication.
 
             Having to individually authenticate into multiple UXP Objects is time-consuming.
             Also time-consuming is  authenticating into a single UXP Object multiple times.
             As a convenience, a UXP Identity can be used for a Single Sign-On session.
             A Single Sign-On session allows automatic authentication into UXP Objects
             that were created using that same UXP Identity.
 
             For authentication, there are two approaches to programatically seeking
             authorization. The first approach is to declare a function callback that
             is called when the system presents challenges. The callback function is
             given the list of challenges that it must process and return. The callback
             function is called until resolution is reached.

             The second uses a looping process to allow the program to handle the
             challenge list manually. Responses are then given back to the system and
             the authentication loop continues until a resolution is reached.

    \author  Greg Smith
    \date    10/30/2019
 
    \editor  Karim Lalani
    \date    06/11/2020

    \note    Application expects to find the necessary source files in the current working folder.
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
 * @brief Checks for an error and logs message.
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
 * Get challenge from the user.
 * @param ch Challenge handle.
 * @return True if accepted. False if cancelled.
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
  uxpFileHandle appHandle;
  uxpIdHandle idHandle;
  long long len;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sampleid.xml";
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
  const char *dataPdfSpec = "$(SAMPLE)/SampleData/data.pdf";
  const char *dataPdf2Spec = "$(SAMPLE)/SampleData/data2.pdf";
  const char *copy1Spec = "$(SAMPLE)/temp/copy1.pdf";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";
  int status = StatusChallenged;
  int cnt, i;

  printf("\nSample SSO using C\n\n");

  /* Allocate a new buffer. Automatically zeros it. */

  buffer = uxpba_newHandle();

  /*
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleSsoC", "Sample SSO 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  /* Create new UXP Object using new UXP Identity */

  appHandle = uxpfile_newHandle();

  uxpfile_openNewFile(appHandle, uxpFileSpec, idFileSpec, IdFile,
                      ModifierReplace, 0);
  checkError(uxpfile_errorStatus(appHandle), "Creating UXP");

  /* Create a new virtual file inside the Data */

  uxpfile_addVirtualFromFile(appHandle, "data.pdf", dataPdfSpec, -1, -1, ModifierCompress);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  uxpfile_addVirtualFromFile(appHandle, "data2.pdf", dataPdf2Spec, -1, -1, 0);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  /* Now, open the first virtual file and write it out to a temporary file */

  uxpOpenVFHandle fileHandle = uxpfile_openVirtualFile(appHandle, "data.pdf", ModeRead);
  checkError(uxpfile_errorStatus(appHandle), "Opening virtual file");

  printf("Reading data.pdf in loop ... ");

  uxpba_setData(buffer, copy1Spec, strlen(copy1Spec));
  uxpsys_processStandardMacros(buffer);

  FILE *fp = fopen(uxpba_getData(buffer), "wb");

  while (uxpfile_readVirtualFile(appHandle, fileHandle, buffer, 100000))
  {
    len = uxpba_getSize(buffer);
    fwrite(uxpba_getData(buffer), len, 1, fp);
  }

  fclose(fp);

  uxpfile_closeVirtualFile(appHandle, fileHandle);

  printf("finished reading data.pdf\n");

  /* Compare the extracted file with the copy inside the Data */

  if (uxpfile_compareExternalFile(appHandle, "data.pdf", copy1Spec))
    printf("Comparison of data.pdf to copy1.pdf: successful\n");
  else
    printf("Comparison of data.pdf to copy1.pdf: failed\n");

  /* Close the UXP Object. This will delete the handle as well. */

  uxpfile_close(appHandle);

  printf("Opening ID for SSO session\n\n");

  printf("Credentials necessary to initiate SSO session:\n");
  printf("  Username = SampleUser@myemail.com\n");
  printf("  Challenge 1 = Response 1\n");
  printf("  Challenge 2 = Response 2\n");
  printf("  ... \n");
  printf("  Challenge 10 = Response 10\n\n");

  idHandle = uxpid_newHandle();

  uxpid_openSessionFromFile(idHandle, idFileSpec, 0, 0);
  checkError(uxpid_errorStatus(idHandle), "Opening ID session");

  while (status != StatusAuthorized)
  {
    status = uxpid_authenticate(idHandle);

    switch (status)
    {
      case StatusAuthorized:
        printf("\nSSO session authorized\n");
        break;

      case StatusChallenged:
        cnt = uxpid_getChallengeCount(idHandle);

        for (i = 0; i < cnt; i++)
        {
          uxpChallengeHandle ch = uxpid_getChallenge(idHandle, i);

          if (!getResponse(ch))
          {
            printf("\nSSO session not authorized\n");
            exit(1);
          }

          uxpid_addResponse(idHandle, ch);

          uxpch_freeHandle(ch);
        }
        break;

      case StatusCanceled:
        printf("SSO session authorization canceled\n");
        exit(1);

      default:
        printf("\nSSO session not authorized\n");
        exit(1);
    }
  }

  /* Reopen the UXP Object ... includes authentication */

  printf("Opening new Data using SSO\n");

  uxpid_openUxpFromFile(idHandle, appHandle, uxpFileSpec, ShareReadOnly);
  checkError(uxpid_errorStatus(idHandle), "Opening UXP from session");

  printf("\nExtracting data2.pdf to copy2.pdf\n");

  uxpfile_exportVirtualFile(appHandle, "data2.pdf", copy2Spec, ModifierReplace);
  checkError(uxpfile_errorStatus(appHandle), "Exporting virtual file");

  if (uxpfile_compareExternalFile(appHandle, "data2.pdf", copy2Spec))
    printf("Comparison of data2.pdf to copy2.pdf: successful\n");
  else
    printf("Comparison of data2.pdf to copy2.pdf: failed\n");

  /* Close up again and free handle */

  uxpfile_close(appHandle);
  checkError(uxpfile_errorStatus(appHandle), "Closing UXP");

  /* Close the SSO session */

  printf("\nSample SSO session closing\n");

  uxpid_closeSession(idHandle);
  checkError(uxpid_errorStatus(idHandle), "Closing ID session");

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpfile_freeHandle(appHandle);
  uxpid_freeHandle(idHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("You successfully authenticated into an Id Session and used it to auto-authenticate into a UXP.\n");
  printf("You may try out other advanced samples now.\n");

  printf("\nSample finished running\n");

  return 0;
}

/**
 * Get challenge from the user.
 * @param ch Challenge handle
 * @return True if accepted. False if cancelled.
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
