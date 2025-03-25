/*! 
    \copyright Sertainty Corporation, 2020  All Rights Reserved.

    \file     sample_workflow_create_id_uxp_open.cpp

    \brief   Demonstrates a mini workflow, resulting in UXP authentication and data extraction
               (1) create ID from XML
               (2) Create UXP from ID & adds data
               (3) Closes the UXP
               (4) Authenticates success / fail
               (5) on success, extracts the data
               (6) closes UXP
 
    \author   Greg Smith
    \date     06/20/2018

    \edited   Melani Smith Weed 
    \date     20-aug-2020
 
    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxplicense.h"
#include "uxpcredential.h"
#include "uxlbytearray.h"
#include "uxpid.h"
#include "uxpidmanager.h"

#include <iostream>

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
  const char *copy1Spec = "$(SAMPLE)/temp/copy1.pdf";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";
  uxl::bytearray idbuf, outbuf, buffer;
  uxp::openVFHandle *handle;
  int status;
  bool done;

  std::cout << std::endl << "Sample C++ UXP Application using ID" << std::endl << std::endl;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("SampleCPP", "Sample C++ 2.0.0");

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

    uxp::file data;

    /* Create a new ID from the sampleid.xml or substituted xml ID definition; must update this sample with specific xml file name. */

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", idbuf);

    uxp::idManager::publishToFile("$(SAMPLE)/temp/sampleid.iic", idbuf, ModifierReplace);

    /* Create new UXP from ID */

    std::cout << "Opening new UXP: " << outfile << std::endl;

    data.openNewFile(outfile, "$(SAMPLE)/temp/sampleid.iic", IdFile, ModifierReplace, 0);
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
          for (auto &t_ch: data.getChallenges())
          {
            uxp::challenge ch(t_ch);

            status = getResponse(ch);
            if (!status)
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

    if (data.compareExternalFile("data.pdf", copy2Spec))
      std::cout << "Comparison of data.pdf to copy2.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy2.pdf: failed" << std::endl;

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
