/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   uxp_from_id.c

    \brief  This sample demonstrates how to create a UXP Object using
            a UXP Identity and add data files into the created UXP Object.

    \author Quinn Carter
    \date   06/19/2020

    \note   Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxlbytearray.h"
#include "uxpsys.h"

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
  
    uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
    uxpFileHandle appHandle;
    const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
    const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
    const char *dataPdfSpec = "$(SAMPLE)/SampleData/data.pdf";
    const char *dataPdf2Spec = "$(SAMPLE)/SampleData/data2.pdf";
    
    std::cout << std::endl << "Creating a Sertainty ID from an ID Definition XML" << std::endl;

    /* Allocate a new buffer. Automatically zeros it. */

    uxl::bytearray buffer;

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

      /* Create new UXP Object from UXP Identity */

      uxp::file data;
      data.openNewFile(uxpFileSpec, idFileSpec, IdFile, ModifierReplace, 0);

      /* Create a new virtual file inside the Data */

      data.addVirtualFileFromFile(std::string("data.pdf"), dataPdfSpec, -1, -1, 0);
      std::cout << "Adding data to UXP" << std::endl;

      data.addVirtualFileFromFile(std::string("data2.pdf"), dataPdf2Spec, -1, -1, 0);
      std::cout << "Adding data to UXP" << std::endl;

      std::cout << std::endl << "You have successfully created a protected UXP and added files into it." << std::endl;
      std::cout << "You can now try to open the UXP to read the contents." << std::endl;
      std::cout << std::endl << "Sample finished running" << std::endl;

      return 0;
    }
    catch (uxl::exception &e2)
    {
      std::cout << "Error: " << e2.getErrorText() << std::endl;

      return 1;
    }
 }
