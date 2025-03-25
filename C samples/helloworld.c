/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file    helloworld.c

    \brief   This sample demonstrates the initialize library call necessary before
             any  UXP Identity or UXP Object functions can be called.

    \author  Karim Lalani
    \date    06/11/2020

    \note
 */

#include "uxlbytearray_c.h"
#include "uxpsys_c.h"

#include <stdio.h>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxlByteArray *buffer;

  printf("\nStarting Sample.\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();

  /*
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "HelloSertainty", "Hello Sertainty 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    printf("Sample failed. Ensure you have a valid installation and license.\n\n");
    return 1;
  }

  printf("Hello, Sertainty!!\n\nYou have a valid license. Try out other samples.\n");

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);

  printf("\nSample finished running\n");

  return 0;
}
