/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   id_from_xml.c

    \brief  This sample demonstrates constructing a UXP Identity file (*.iic) from an ID Definition XML
            Source file (*.xml). The generated UXP Identity file (*.iic) can be used to generate a UXP Object.

    \author Karim Lalani
    \date   06/11/2020

    \note   Application expects to find the necessary source files in the current working folder.
 */

#include "uxlbytearray_c.h"
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
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxlByteArray *buffer;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sampleid.xml"; // ID Definition XML Source
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic"; // UXP Identity file name to be created

  printf("\nCreating a Sertainty ID from an ID Definition XML\n\n");

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

  /* Create a new UXP Identity from the sampleid.xml ID definition */

  uxpsys_fileReadAll(callstatus, idXmlSpec, buffer);
  checkError(callstatus, "Reading ID XML");

  uxpid_publishToFile(callstatus,
                      idFileSpec,
                      uxpba_getData(buffer),
                      ModifierReplace);
  checkError(callstatus, "Creating Sertainty ID");

  printf("Your ID is now created. Try protecting a file with it.\n");

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("\nSample finished running\n");

  return 0;
}
