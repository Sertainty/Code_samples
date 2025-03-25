/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   helloworld.cpp

    \brief  This sample demonstrates the initialize library call necessary before
            any UXP  Identity or UXP Object functions can be called.

    \author Quinn Carter
    \date   06/16/2020

    \note   Application expects to find the necessary source files in the current working folder.
 */

 #include "uxlbytearray.h"
 #include "uxpsys.h"
 #include "uxlmessage.h"

 #include <iostream>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
 int main(int argc, char *argv[])
 {
  bool licensed;

  std::cout << std::endl << "Starting Sample" << std::endl;

  /* 
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  try
  {
    uxp::sys::initializeLibrary(argc, argv,"sertainty.lic","SertintyONE");
    licensed = true;
  }
  catch (uxl::exception &)
  {
    licensed = false;
  }

  uxp::sys::setLogging(uxpLogInfo);

  if (!licensed)
  {
    std::cout << "Error Error initializing environment" << std::endl;
    std::cout << "Sample failed. Ensure you have a valid installation and license." << std::endl;

    return 1;
  }

  std::cout << std::endl << "Hello, Sertainty!!" << std::endl << std::endl <<"You have a valid license. Try out other samples." << std::endl;

  return 0;
 }
