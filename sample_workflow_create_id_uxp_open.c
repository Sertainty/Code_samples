/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_workflow_create_id_uxp_open.c

    \brief   Demonstrates a mini workflow, resulting in UXP authentication and data extraction
               (1) create ID from XML
               (2) Create UXP from ID & adds data
               (3) Closes the UXP
               (4) Authenticates success / fail
               (5) on success, extracts the data
               (6) closes UXP

    \author   Greg Smith
    \date     06/20/2018

    \edited   Melani Smith Weed
    \date     20-aug-2020
 
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
 * @return True if accepted.  False if canceled
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
  long long len;
  int status;
  int done,i;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sampleid.xml";
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
  const char *dataPdfSpec = "$(SAMPLE)/SampleData/data.pdf";
  const char *dataPdf2Spec = "$(SAMPLE)/SampleData/data2.pdf";
  const char *copy1Spec = "$(SAMPLE)/temp/copy1.pdf";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";

  printf("\nSample C Data Application using ID\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();

  /* Set up log file and initialize library.  Must do this before any active calls. */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleC", "Sample 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  /* Create a new ID from the sampleid.xml or substituted xml ID definition; must update this sample with specific xml file name */

  uxpsys_fileReadAll(callstatus, idXmlSpec, buffer);
  checkError(callstatus, "Reading ID");

  uxpid_publishToFile(callstatus,
                      idFileSpec,
                      uxpba_getData(buffer),
                      ModifierReplace);
  checkError(callstatus, "Creating ID");

  /* Create new UXP from ID */

  appHandle = uxpfile_newHandle();

  uxpfile_openNewFile(appHandle, uxpFileSpec, idFileSpec, IdFile,
                      ModifierReplace, 0);
  checkError(uxpfile_errorStatus(appHandle), "Creating new UXP");

  /* Create a new virtual file inside the Data */

  uxpfile_addVirtualFromFile(appHandle, "data.pdf", dataPdfSpec, -1, -1, ModifierCompress);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  uxpfile_addVirtualFromFile(appHandle, "data2.pdf", dataPdf2Spec, -1, -1, 0);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  /* Now, open the first virtule file and write it out to a temporary file */

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
  checkError(uxpfile_errorStatus(appHandle), "Close virtual file");

  printf("finished reading data.pdf\n");

  /* Compare the extracted file with the copy inside the Data */

  if (uxpfile_compareExternalFile(appHandle, "data.pdf", copy1Spec))
    printf("Comparison of data.pdf to copy1.pdf: successful\n");
  else
    printf("Comparison of data.pdf to copy1.pdf: failed\n");

  /* Close the UXP. This will delete the handle as well. */

  uxpfile_close(appHandle);

  /* Reopen the Data ... includes authentication */

  printf("Opening new Data\n\n");

  uxpfile_openFile(appHandle, uxpFileSpec, ShareReadOnly);
  checkError(uxpfile_errorStatus(appHandle), "Opening UXP");

  printf("Credentials necessary to access UXP:\n");
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

  fileHandle = uxpfile_openVirtualFile(appHandle, "data2.pdf", ModeRead);
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

  if (uxpfile_compareExternalFile(appHandle, "data2.pdf", copy2Spec))
    printf("Comparison of data2.pdf to copy2.pdf: successful\n");
  else
    printf("Comparison of data2.pdf to copy2.pdf: failed\n");

  /* Close up again and free handle */

  uxpfile_close(appHandle);

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
 * @return True if accepted.  False if canceled
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
