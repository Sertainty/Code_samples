/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file    uxp_from_id.c

    \brief   This sample demonstrates how to create a UXP Object using a
             UXP Identity and add data files into the created UXP Object.

    \author  Greg Smith
    \date    06/20/2018
 
    \editor  Karim Lalani
    \date    06/11/2020

    \note    Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile_c.h"
#include "uxlbytearray_c.h"
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
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpFileHandle appHandle;
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
  const char *dataPdfSpec = "$(SAMPLE)/SampleData/data.pdf";
  const char *dataPdf2Spec = "$(SAMPLE)/SampleData/data2.pdf";

  printf("\nCreating a UXP file using a Sertainty ID\n\n");

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

  /* Create new UXP Object from UXP Identity */

  appHandle = uxpfile_newHandle();

  uxpfile_openNewFile(appHandle, uxpFileSpec, idFileSpec, IdFile,
                      ModifierReplace, 0);
  checkError(uxpfile_errorStatus(appHandle), "Creating new UXP");

  /* Create a new virtual file inside the Data */

  uxpfile_addVirtualFromFile(appHandle, "data.pdf", dataPdfSpec, -1, -1, ModifierCompress);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  uxpfile_addVirtualFromFile(appHandle, "data2.pdf", dataPdf2Spec, -1, -1, 0);
  checkError(uxpfile_errorStatus(appHandle), "Adding data to UXP");

  /* Close up again and free handle */

  uxpfile_close(appHandle);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpfile_freeHandle(appHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("You have successfully created a protected UXP and added files into it.\n");
  printf("You can now try to open the UXP to read the contents.\n");

  printf("\nSample finished running\n");

  return 0;
}
