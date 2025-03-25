/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file   open_uxp_auth.cpp

    \brief  This sample shows how to open and interactively authenticate a
            UXP Object using known prompt/response pairs and read contents
            of virtual files protected within a UXP Object.
 
    \author Quinn Carter
    \date   07/09/2020

    \note   Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxpfile_common.h"
#include "uxlbytearray.h"
#include "uxpcredential.h"
#include "uxpid.h"
#include "uxpsys.h"

#include <iostream>

/**
 * Response from the user.
 * @param ch Challenge object
 * @return True if accepted. False if cancelled.
 */
static int getResponse(uxp::challenge &ch);

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
   const char *uxpFileSpec = "$(SAMPLE)/temp/sample.uxp";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";
  int status;
  bool done;

  std::cout << std::endl << "Opening and Authenticating into a UXP interactively" << std::endl << std::endl;

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

    uxp::file data;

    std::cout << "Opening new data." << std::endl;
    std::cout << "File Open." << std::endl;
    std::cout << "Checking for Errors." << std::endl << std::endl;

    std::cout << "Credentials necessary to access UXP:" << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;

    data.openFile(uxpFileSpec, ShareAll);

    done = false;
    
    /* Continue to authenticate until a final decision is reached */

    while (!done)
    {
      status = data.authenticate();

      switch (status)
      {
        case StatusAuthorized:
          std::cout << " - You are authorized" << std::endl;
          done = true;
          break;

        case StatusNotAuthorized:
          std::cout << " - You are not authorized" << std::endl;
          return 1;

        case StatusChallenged:
          for (auto &t_ch: data.getChallenges())
          {
            uxp::challenge ch(t_ch);

            status = getResponse(ch);
            if (!status)
            {
              std::cout << "Authorization canceled" << std::endl;
              data.close();

              return 1;
            }

            data.addResponse(ch);
          }
          break;

        default:
          break;
      }
    }

    std::cout << std::endl << "Opening virtual file" << std::endl;
    
    uxp::openVFHandle *handle = data.openVirtualFile("data.pdf", ModeRead);

    buffer.assign(copy2Spec);
    uxp::sys::processStandardMacros(buffer);

    FILE *f1 = fopen(buffer.data(), "wb");

    while (data.readVirtualFile(handle, buffer))
    {
      fwrite(buffer.data(), static_cast<size_t>(buffer.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    if (data.compareExternalFile("data.pdf", copy2Spec))
      std::cout << "Comparison of data.pdf to copy2.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy2.pdf: failed" << std::endl;

    /* Close up again and free handle */
      
    data.close();

    std::cout << "\nYou have successfully opened and interactively authenticated into a UXP." << std::endl;
    std::cout << "You may try out other advanced samples now." << std::endl;

    std::cout << std::endl << "Sample finished running" << std::endl;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;

    return 1;
  }
}

/**
 * Response from the user
 * @param ch Challenge object
 * @return True if accepted. False if cancelled.
 */
static int getResponse(uxp::challenge &ch)
{
  uxl::string prompt(ch.getPrompt());

  ch.startTimer();

  if (prompt == "Username")
  {
    ch.setValue("SampleUser@myemail.com");
  }
  else
  {
    ch.setValue(uxl::string::format("Response %1", prompt.mid(10)));
  }

  ch.endTimer();

  return 1;
}
