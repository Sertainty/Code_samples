/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_workflow_auto_auth.c

    \brief   Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
                (1) create ID from XML
                (2) Create UXP from ID & adds data
                (3) Closes the UXP
                (4) Auto authenticates success / fail
                (5) on success, extracts the data
                (6) closes UXP

    \author   Greg Smith
    \date     06/20/2018
 
    \edited  Melani Smith Weed 
    \date    27-jul-2020

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
#include "uxlmessage_c.h"

#include "sample_auto_auth_text.h"

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
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpFileHandle appHandle;
  int status;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sample_auto_auth_id.xml";
  const char *idFileSpec = "$(SAMPLE)/temp/sample_auto_auth_id.iic";
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample_auto_auth.uxp";
  int i;

  printf("\n\nSample Auto-Open C Data Application\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();

  /* Set up log file and initialize library. Must do this before any active calls. */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleAutoOpenC", "Sample AutoOpen 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  printf("Files of interest:\n");
  printf("  sample_auto_auth_id.xml     - Sample ID source documnt.  IIC is created from this.\n");
  printf("  sample_workflow_auto_auth.c          - Sample source file for C\n");
  printf("  sample_auto_auth_text.xml   - Secure text source document\n");
  printf("  sample_auto_auth_text.c     - Secure text C file ... auto-generated\n");
  printf("  sample_auto_auth_text.h     - Secure text header file ... auto-generated\n");
  printf("  sample_auto_auth_text.msg   - Secure text ... auto-generated ... must be in Sertainty home\n\n");

  printf("Build notes:\n");
  printf("  Compile secure text using SertaintyString utility.\n");
  printf("  Compile sample_workflow_auto_auth.c\n");
  printf("  Compile sample_auto_auth_text.c\n");
  printf("  Link the two compiled C files along with Sertainty library.\n\n");

  /* 
      This will initialize the secure text.  We store the necessary challenges
      and responses from the secure text message file in the Sertainty home folder. 
  */

  sample_auto_auth_text_init();

  /* 
      Create a new ID from the sample_auto_auth_id.xml ID definition.

      Note: an ID can be created one time and reused many times.
      This sample is just a demonstration of the complete workflow.
  */

  uxpsys_fileReadAll(callstatus, idXmlSpec, buffer);
  checkError(callstatus, "Reading sample ID XML");

  uxpid_publishToFile(callstatus,
                      idFileSpec,
                      uxpba_getData(buffer),
                      ModifierReplace);
  checkError(callstatus, "Creating ID");

  /* Create new UXP from ID */

  printf("Creating new UXP: %s\n", uxpFileSpec);
  printf("Protecting document data.pdf\n");

  appHandle = uxpfile_newHandle();

  uxpfile_openNewFile(appHandle, uxpFileSpec, idFileSpec, IdFile,
                      ModifierReplace, 0);
  checkError(uxpfile_errorStatus(appHandle), "Creating UXP");

  /* Create a new virtual file inside the Data */

  uxpfile_addVirtualFromFile(appHandle, "data.pdf", "data.pdf", -1, -1, ModifierCompress);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  uxpfile_close(appHandle);
  checkError(uxpfile_errorStatus(appHandle), "Closing new UXP");

  /* Reopen the UXP ... includes auto authentication */

  uxpfile_openFile(appHandle, uxpFileSpec, ShareReadOnly);
  checkError(uxpfile_errorStatus(appHandle), "Opening UXP");

  printf("Opening new UXP and auto-authenticating\n");

  /* Push challenges/responses to UXP for auto-open */

  uxpChallengeHandle ch = uxpch_newHandle();

  uxlmsg_getSecureText(callstatus, buffer, "Sample", SAMPLE_T_USER);

  uxpch_setName(ch, "USERNAME");
  uxpch_setPrompt(ch, "Username");
  uxpch_setValueString(ch, uxpba_getData(buffer));

  uxpfile_addResponse(appHandle, ch);

  for (i = 0; i < 10; i++)
  {
    uxlmsg_getSecureText(callstatus, buffer, "Sample", SAMPLE_T_CH0 + i);

    uxpch_setName(ch, uxpba_getData(buffer));
    uxpch_setPrompt(ch, uxpba_getData(buffer));

    uxlmsg_getSecureText(callstatus, buffer, "Sample", SAMPLE_T_RS0 + i);

    uxpch_setValueString(ch, uxpba_getData(buffer));

    uxpfile_addResponse(appHandle, ch);
  }

  uxpch_freeHandle(ch);

  status = uxpfile_authenticate(appHandle, 0);

  switch (status)
  {
    case StatusAuthorized:
      printf("Access has been authorized\n");
      break;

    default:
    case StatusNotAuthorized:
      printf("Access denied\n");
      return 1;
  }

  printf("Extracting data.pdf to copy.pdf\n");

  uxpfile_exportVirtualFile(appHandle, "data.pdf", "copy.pdf", ModifierReplace);
  checkError(uxpfile_errorStatus(appHandle), "Exporting virtual file");

  uxpfile_close(appHandle);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpfile_freeHandle(appHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("\nSample finished running\n");

  return 0;
}
