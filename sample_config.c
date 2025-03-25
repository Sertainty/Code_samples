/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_config.c

    \brief    Sample program using C language interface that demonstrates
              fetching current machine's location information using the
              Sertainty SDK. Similarly, machine's network interface information
              can also be fetched from the machine's configuration. This sample
              only demonstrates location information for brevity.

    \author   Karim Lalani
    \date     07/07/2020

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
#include "uxpconfig_c.h"
#include "uxplocation_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxlByteArray *buffer, *ba_name, *ba_value;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();

  printf("\nSample C Config/Location Application\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();

  /*
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleCConfig", "Sample C Config 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  /* Load the current machine configuration to extract location information */

  uxpConfigHandle config = uxpcf_getCurrentConfig(0);
  if(uxpsys_hasError(uxpcf_errorStatus(config)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpcf_errorStatus(config));
    printf("Error getting Config: %s\n",errMsg);
    return 1;
  }

  /* Extract location config from machine config */

  uxpLocationHandle location = uxpcf_getLocation(config);
  if(uxpsys_hasError(uxpcf_errorStatus(config)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpcf_errorStatus(config));
    printf("Error getting Current Location: %s\n",errMsg);
    return 1;
  }

  /* Get the Zipcode from location config */

  printf("Zipcode Property from location: %s\n\n",uxploc_getProperty(location,"Zipcode"));

  /* Get the location config table to read location data iteratively */

  uxpListHandle table = uxploc_getTable(location);
  if(uxpsys_hasError(uxpcf_locationErrorStatus(location)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpcf_locationErrorStatus(location));
    printf("Error getting location table: %s\n",errMsg);
    return 1;
  }

  /* Get property count on location config table */

  int tableCount = uxplist_count(table);

  ba_name = uxpba_newHandle();
  ba_value = uxpba_newHandle();
  for(int i = 0; i < tableCount; i++)
  {
    /* Fetch location config property row at index i */

    uxlVariantHandle row = uxplist_getVariable(table,i);

    /* Get the location config property name from row */

    uxpvar_getName(row,ba_name);

    /* Get the location config property value from row as string */

    uxpvar_toString(row,ba_value);

    printf("%s: %s\n",uxpba_getData(ba_name),uxpba_getData(ba_value));

    /* Clear buffer */

    uxpba_clearData(ba_name);
    uxpba_clearData(ba_value);
  }

  uxplist_freeList(table);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpba_freeHandle(ba_name);
  uxpba_freeHandle(ba_value);
  uxploc_freeHandle(location);
  uxpcf_freeHandle(config);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("\nSample finished running\n");

  return 0;
}
