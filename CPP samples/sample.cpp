/*! 
    \copyright Sertainty Corporation, 2016.  All Rights Reserved.

    \file     sample.cpp

    \brief  Sertainty sample program using native C++ language interface.

    \author   Greg Smith
    \date     06/20/2016
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"

#include <string.h> 

/**
 * Response from the user
 * @param ch Challenge object
 * @return True if accepted.  False if canceled
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
  std::string outfile("$(SAMPLE)/temp/sample.uxp");
  uxl::bytearray outbuf, buffer, kclfile("$(SAMPLE)/SampleData/sample.kcl");
  const char *copy1Spec = "$(SAMPLE)/temp/copy1.pdf";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";
  uxp::openVFHandle *handle;
  int status;
  bool done;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("sample c++", "Sample c++ 1.0.0");

    /* Attempt to find the license file */

    try
    {
      uxp::sys::initializeLibrary(argc, argv,
                                  "sertainty.lic",
                                  "SertintyONE");
      licensed = true;
    }
    catch (uxl::exception &e)
    {
      licensed = false;
    }

    uxp::sys::setLogging(uxpLogInfo);

    if (!licensed)
    {
      std::cout << "UXP-432, A valid license was not found." << std::endl;

      return 1;
    }

    uxp::file data;

    std::cout << std::endl << "Sample C++ UXP Application" << std::endl << std::endl;

    /* Create new UXP.  We are showing KCL; however, can also use ID to create UXP. */

    std::cout << "Opening new UXP: " << outfile << std::endl;

    data.openNewFile(outfile, kclfile, KclFile, ModifierReplace, 0);
    data.addVirtualFileFromFile("data.pdf", std::string("$(SAMPLE)/SampleData/data.pdf"), -1, -1, 0);

    /* Read the virtual file and write it back out to disk */

    std::cout << "Extracting data.pdf to copy1.pdf" << std::endl;

    handle = data.openVirtualFile("data.pdf", ModeRead);

    buffer.assign(copy1Spec);
    uxp::sys::processStandardMacros(buffer);

    FILE *f1 = fopen(buffer.data(), "wb");
    
    while (data.readVirtualFile(handle, outbuf))
    {
      fwrite(outbuf.data(), static_cast<size_t>(outbuf.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    if (data.compareExternalFile("data.pdf", copy1Spec))
      std::cout << "Comparison of data.pdf to copy1.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy1.pdf: failed" << std::endl;

    /* Close the UXP */

    data.close();

    /* Reopen the UXP ... includes authentication */

    std::cout << "Opening new UXP" << std::endl << std::endl;

    std::cout << "Credentials necessary to access UXP:" << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;

    data.openFile(outfile, ShareAll);

    done = false;

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
          for (uxp::challenge ch: data.getChallenges())
          {
            if (!getResponse(ch))
            {
              std::cout << "Canceled" << std::endl;
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

    /* Read the virtual file and write it back out to disk */

    std::cout << std::endl << "Extracting data.pdf to copy2.pdf" << std::endl;

    handle = data.openVirtualFile("data.pdf", ModeRead);

    buffer.assign(copy2Spec);
    uxp::sys::processStandardMacros(buffer);
    
    f1 = fopen(buffer.data(), "wb");

    while (data.readVirtualFile(handle, outbuf))
    {
      fwrite(outbuf.data(), static_cast<size_t>(outbuf.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    data.close();

    data.close();

    std::cout << std::endl << "Sample finished running" << std::endl;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;
  }

  return 0;
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
