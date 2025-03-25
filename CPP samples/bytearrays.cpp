/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   bytearrays.cpp

    \brief  This sample demonstrates the use of uxlByteArray,
            which is a UXP Data Structure that is used to handle binary and ascii data.

            Demonstrated in the sample are writing data to and reading data from the
            uxlByteArray as well as loading contents of a file read from the
            Virtual File System into the uxlByteArray.

    \author Quinn Carter
    \date   06/26/2020

    \note
 */

#include "uxlbytearray.h"
#include "uxpsys.h"

#include <string.h> 
#include <iostream>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  std::string fileSpec = "$(SAMPLE)/temp/temp.txt";

  std::cout << std::endl << "UXL Byte Arrays" << std::endl << std::endl;

  /* Allocate a new buffer. Automatically zeros it. */

  uxl::bytearray buffer;

  char const chArr[] = "Hello, Sertainty! in char array.";
  int lArr = strlen(chArr);
  char const hPtr [35] = "Hello, Sertainty! in char pointer.";
  int lPtr = strlen(hPtr);
  char const chPtrForFile[52] = "ASCII Content for file. This can be binary as well.";
  int lPtrForFile = strlen(chPtrForFile); 

  buffer.append(chArr,lArr);
  const char * dataFromArr = buffer.data();

  std::cout << "Array  Size: "<< lArr << std::endl;
  std::cout << "Buffer Size: " <<  buffer.size() << std::endl;

  std::cout << "Array  Contents: " << chArr << std::endl;
  std::cout << "Buffer Contents: " << dataFromArr << std::endl << std::endl;

  buffer.clear();
  buffer.append(hPtr,lPtr);

  std::cout << "Ch Ptr Size: " << lPtr << std::endl;
  std::cout << "Buffer Size: " <<  buffer.size() << std::endl;

  std::cout << "Ch Ptr Contents: " << hPtr << std::endl;
  std::cout << "Buffer Contents: " << buffer.data() << std::endl << std::endl;

  buffer.clear();
  buffer.append(chPtrForFile,lPtrForFile);
  uxp::sys::fileWriteAll(fileSpec,buffer,ModifierReplace);

  try
  {
    printf("File written to disk successfully.\n");
    uxp::sys::fileReadAll(fileSpec,buffer);
    try
    {
      std::cout << "Content Size: " << lPtrForFile << std::endl;
      std::cout << "Buffer  Size: " <<  buffer.size() << std::endl;

      std::cout << "Contents       : " << chPtrForFile << std::endl;
      std::cout << "Buffer Contents: " << buffer.data() << std::endl << std::endl;
    }
    catch(uxl::exception &e)
    {
      std::cout << "Error reading file into buffer: " << e.getErrorText() << std::endl;
      return 1;
    }
  }
  catch(uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;
    return 1;
  }

  std::cout << "Sample finished running" << std::endl;

  return 0;
}
