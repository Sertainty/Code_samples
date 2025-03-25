/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_workflow_auto_auth.cpp

    \brief    Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
    \                    (1) create ID from XML
    \                    (2) Create UXP from ID & adds data
    \                    (3) Closes the UXP
    \                    (4) Auto authenticates success / fail
    \                    (5) on success, extracts the data
    \                    (6) closes UXP 

    \author   Greg Smith
    \date     06/20/2018
 
    \edited   Melani Smith Weed 
    \date     27-jul-2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"
#include "uxpid.h"
#include "uxpidmanager.h"

#include "sample_auto_auth_text.h"

#include <iostream>
#include <stdio.h>


/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  bool licensed;
  std::string outfile("sample_auto_auth.uxp");
  uxl::bytearray idbuf, outbuf;

  std::cout << std::endl << std::endl << "Sample Auto-Open C++ UXP Application" << std::endl << std::endl;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("SampleAutoOpenCPP", "Sample AutoOpen C++ 2.0.0");

    /* Attempt to find the license file */

    try
    {
      uxp::sys::initializeLibrary(argc, argv,
                                  "sertainty.lic",
                                  "SertintyONE");
      licensed = true;
    }
    catch (uxl::exception &)
    {
      licensed = false;
    }

    uxp::sys::setLogging(uxpLogInfo);

    if (!licensed)
    {
      std::cout << "UXP-432, A valid license was not found." << std::endl;

      return 1;
    }

    std::cout << "Files of interest:" << std::endl;
    std::cout << "  sample_auto_auth_id.xml     - Sample ID source documnt.  IIC is created from this."  << std::endl;
    std::cout << "  sample_workflow_auto_auth.cpp        - Sample source file for C++"  << std::endl;
    std::cout << "  sample_auto_auth_text.xml   - Secure text source document"  << std::endl;
    std::cout << "  sample_auto_auth_text.c     - Secure text C file ... auto-generated"  << std::endl;
    std::cout << "  sample_auto_auth_text.h     - Secure text header file ... auto-generated"  << std::endl;
    std::cout << "  sample_auto_auth_text.msg   - Secure text ... auto-generated ... must be in Sertainty home"  << std::endl << std::endl;

    std::cout << "Build notes:"  << std::endl;
    std::cout << "  Compile secure text using SertaintyString utility." << std::endl;
    std::cout << "  Compile sample_workflow_auto_auth.cpp"  << std::endl;
    std::cout << "  Compile sample_auto_auth_text.c"  << std::endl;
    std::cout << "  Link the two compiled C++ files along with Sertainty library."  << std::endl << std::endl;

    /* 
        This will initialize the secure text.  We store the necessary challenge
        and responses from the secure text message file in the Sertainty home folder. 
    */

    sample_auto_text_init();

    /* 
        Create a new ID from the sample_auto_auth_id.xml ID definition.

        Note: an ID can be created one time and reused many times.  
        This sample is just a demonstration of the complete workflow.
    */

    uxp::sys::fileReadAll("sample_auto_auth_id.xml", idbuf);

    uxp::idManager::publishToFile("sample_auto_auth_id.iic", idbuf, ModifierReplace);

    /* Create new UXP from ID */

    std::cout << "Creating new UXP: " << outfile << std::endl;
    std::cout << "Protecting document data.pdf" << std::endl;

    uxp::file data;
    uxp::challenge ch;

    data.openNewFile(outfile, "sample_auto_auth_id.iic", IdFile, ModifierReplace, 0);
    data.addVirtualFileFromFile("data.pdf", std::string("data.pdf"), -1, -1, 0);
    data.close();

    std::cout << "Closing new UXP" << std::endl;

    /* Reopen the UXP ... includes auto authentication */

    std::cout << "Opening new UXP and auto-authenticating" << std::endl;

    data.openFile(outfile, ShareAll);

    /* Push challenge/responses to UXP for auto-open */

    ch.setName("USERNAME");
    ch.setValue(uxl::message::getSecureText("Sample",SAMPLE_T_USER));

    data.addResponse(ch);

    for (uxp_int16 i = 0; i < 10; i++)
    {
      ch.setName(uxl::message::getSecureText("Sample", SAMPLE_T_CH0 + i));
      ch.setValue(uxl::message::getSecureText("Sample", SAMPLE_T_RS0 + i));

      data.addResponse(ch);
    }

    switch (data.authenticate())
    {
      case StatusAuthorized:
        std::cout << "Access has been authorized" << std::endl;
        break;

      default:
      case StatusNotAuthorized:
        std::cout << "Access denied" << std::endl;
        return 1;
    }

    /* Read the virtual file and write it back out to disk */

    std::cout << "Extracting data.pdf to copy.pdf" << std::endl;

    data.exportVirtualFile("data.pdf", "copy.pdf", ModifierReplace);

    data.close();

    std::cout << std::endl << "Sample finished running" << std::endl;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;
  }

  return 0;
}
