/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file   bytearrays.c

    \brief  This sample demonstrates the use of uxlByteArray, which is a
            UXP Data  Structure that is used to handle binary and ascii data.

            Demonstrated in the sample are writing data to and reading data
            from the uxlByteArray as well as loading contents of a file read
            from the Virtual File system into the uxlByteArray.

    \author Karim Lalani
    \date   06/11/2020

    \note
 */

#include "uxlbytearray_c.h"
#include "uxpsys_c.h"

#include <stdio.h>
#include <string.h>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxlByteArray *buffer;
  uxpCallStatusHandle callStatus = uxpsys_newCallStatusHandle();
  const char * fileSpec = "$(SAMPLE)/temp/temp.txt";

  printf("\nUXL Byte Arrays\n\n");

  /* Allocate a new buffer. Automatically zeros it. */

  buffer = uxpba_newHandle();

  char chArr[] = "Hello, Sertainty! in char array.";
  size_t lArr = strlen(chArr);
  char * chPtr = "Hello, Sertainty! in char pointer.";
  size_t lPtr = strlen(chPtr);
  char * chPtrForFile = "ASCII Content for file. This can be binary as well.";
  size_t lPtrForFile = strlen(chPtrForFile); // strlen might not work for Binary content. Use pre-allocated buffer with memory etc.

  uxpba_setData(buffer, chArr,lArr);
  const char * dataFromArr = uxpba_getData(buffer);

  printf("Array  Size: %zu\n", lArr);
  printf("Buffer Size: %lld\n", uxpba_getSize(buffer));

  printf("Array  Contents: %s\n",chArr);
  printf("Buffer Contents: %s\n\n",dataFromArr);

  uxpba_setData(buffer, chPtr,lPtr);
  const char * dataFromPtr = uxpba_getData(buffer);

  printf("Ch Ptr Size: %zu\n", lPtr);
  printf("Buffer Size: %lld\n", uxpba_getSize(buffer));

  printf("Ch Ptr Contents: %s\n",chPtr);
  printf("Buffer Contents: %s\n\n",dataFromPtr);

  uxpsys_fileWriteAll(callStatus,fileSpec,chPtrForFile,lPtrForFile,ModifierReplace);
  if(uxpsys_hasError(callStatus))
  {
    printf("Error writing buffer to file: %s\n",uxpsys_getErrorMessage(callStatus));
  }
  else
  {
    printf("File written to disk successfully.\n");
    uxpsys_fileReadAll(callStatus,fileSpec,buffer);

    if(uxpsys_hasError(callStatus))
    {
      printf("Error reading file into buffer: %s\n",uxpsys_getErrorMessage(callStatus));
    }
    else
    {
      const char * dataFromFile = uxpba_getData(buffer);

      printf("Content Size: %zu\n", lPtrForFile);
      printf("Buffer  Size: %lld\n", uxpba_getSize(buffer));

      printf("Contents       : %s\n",chPtrForFile);
      printf("Buffer Contents: %s\n",dataFromFile);
    }
  }

  uxpba_freeHandle(buffer);

  printf("\nSample finished running\n");

  return 0;
}
