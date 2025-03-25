/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   id_from_xml.cpp

    \brief  This sample demonstrates constructing a UXP Identity file  (*.iic )from an
            ID Definition XML Source file (*.xml). The generated UXP Identity file (*.iic)
            can be used to generate a UXP Object.

    \author Quinn Carter
    \date   06/18/2020

    \note   Application expects to find the necessary source files in the current working folder.
 */
 
 #include "uxlbytearray.h"
 #include "uxpsys.h"
 #include "uxpid.h"
 #include "uxpidmanager.h"

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
  uxl::bytearray buffer;
  std::cout << std::endl << "Creating a Sertainty ID from an ID Definition XML" << std::endl;

  /*
      Set up log file and initialize library.
      Must do this before any active calls.
  */

  try
  {
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

    /* Create a new UXP Identity from the sampleid.xml ID definition. */
      
    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", buffer); // ID Definition XML Source
    uxp::idManager::publishToFile("$(SAMPLE)/temp/sampleid.iic",buffer,ModifierReplace); // UXP Identity file name to be created

    std::cout << std::endl << "Your ID is now created. Try protecting a file with it." << std::endl;

    std::cout << std::endl <<  "Sample finished running" << std::endl;

    return 0;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;
    
    return 1;
  }
 }
