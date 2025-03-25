/*! \copyright Sertainty Corporation, 2018.  All Rights Reserved.

    \file     sample_sso.cpp

    \brief  Sertainty single sign-on sample program using native C++ language interface.
            Uses XML to create ID.  New ID is used as a single sign-on object.

    \details  A UXP requires authentication of the prospective user.  Access will only be
              granted after a successful authentication.  The process of authentication can
              be tedious if a user accesses multiple UXP entities or the same UXP entity
              multiple times.  As a convenience, a Sertainty ID can be used as a single
              sign-on session, which will automatically authenticate UXP entities that
              were created using the ID.

              For authentication, there are two approaches to programatically seeking
              authorization.  The first approach is to declare a function callback that
              is called when the system presents challenges.  The callback function is
              given the list of challenges that it must process and return.  The callback
              function is called until resolution is reached.

              The second uses a looping process to allow the program to handle the
              challenge list manually.  Responses are then given back to the system and
              the authentication loop continues until a resolution is reached.

    \author   Greg Smith
    \date     10/30/2019

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
 * @brief Response from the user
 * @param ch Challenge object
 * @param prefix Response prefix
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxp::challenge &ch, const std::string &prefix);

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
  uxl::bytearray idbuf, outbuf, buffer;
  uxp::openVFHandle *handle;
  int status = StatusChallenged;

  std::cout << std::endl << std::endl << "Sample SSO session using C++" << std::endl << std::endl;

  /* Set up log file and initialize library.  Must do this before any active calls. */

  try
  {
    uxp::sys::setLogFile("SampleSsoCPP", "Sample SSO C++ 2.0.0");

    /* Attempt to find the license file. */

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

    /* Create a new ID from the sampleid.xml ID definition */

    std::cout << "Creating new ID: " << "sampleid.iic" << std::endl;

    uxp::sys::fileReadAll("$(SAMPLE)/SampleData/sampleid.xml", idbuf);

    uxp::idManager::publishToFile("$(SAMPLE)/temp/sampleid.iic", idbuf, ModifierReplace);

    /* Create new UXP using ID */

    std::cout << "Opening new UXP: " << outfile << std::endl;

    uxp::file data;

    data.openNewFile(outfile, "$(SAMPLE)/temp/sampleid.iic", IdFile, ModifierReplace, 0);
    data.addVirtualFileFromFile("data.pdf", std::string("$(SAMPLE)/SampleData/data.pdf"), -1, -1, 0);

    /* Read the virtual file and write it back out to disk */

    std::cout << "Extracting data.pdf to copy1.pdf" << std::endl;

    handle = data.openVirtualFile("data.pdf", ModeRead);

    buffer.assign("$(SAMPLE)/temp/copy1.pdf");
    uxp::sys::processStandardMacros(buffer);

    FILE *f1 = fopen(buffer.data(), "wb");

    while (data.readVirtualFile(handle, outbuf))
    {
      fwrite(outbuf.data(), static_cast<size_t>(outbuf.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    if (data.compareExternalFile("data.pdf", "$(SAMPLE)/temp/copy1.pdf"))
      std::cout << "Comparison of data.pdf to copy1.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy1.pdf: failed" << std::endl;

    /* Close the UXP */

    data.close();

    /* Start an SSO session ... includes authentication. */

    std::cout << "Opening ID for SSO session" << std::endl << std::endl;

    std::cout << "Credentials necessary to initiate SSO session:" << std::endl << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;

    uxp::id sampleid;

    try
    {
      sampleid.openSessionFile(std::string("$(SAMPLE)/temp/sampleid.iic"));

      /* Authenticate until we have resolution. */

      while (status != StatusAuthorized)
      {
        status = sampleid.authenticate();

        switch (status)
        {
          case StatusAuthorized:
            std::cout << std::endl << "SSO session authorized" << std::endl;
            break;

          case StatusChallenged:
            for (uxp::challenge ch: sampleid.getChallenges())
            {
              if (!getResponse(ch, "Response"))
              {
                std::cout << "SSO session authorization canceled" << std::endl;
                exit(1);
              }

              sampleid.addResponse(ch);
            }
            break;

          case StatusCanceled:
            std::cout << "SSO session authorization canceled" << std::endl;
            exit(1);

          default:
            std::cout << "SSO session not authorized" << std::endl;
            exit(1);
        }
      }
    }
    catch (std::exception &e)
    {
      std::cout << "SSO session not authorized" << std::endl;
      std::cout << "Reason: " << e.what() << std::endl;

      exit(1);
    }

    /* Open our sample UXP using the current ID session.  If the UXP was created using the ID,
       it should open without prompting the user. */

    std::cout << "Opening UXP using SSO" << std::endl;

    status = sampleid.openUxp(data, outfile, ShareAll);
    if (status == StatusAuthorized)
      std::cout << "UXP access authorized" << std::endl;
    else
      std::cout << "UXP access not authorized" << std::endl;

    /* Read the virtual file and write it back out to disk */

    std::cout << std::endl << "Extracting data.pdf to copy2.pdf" << std::endl;

    handle = data.openVirtualFile("data.pdf", ModeRead);

    buffer.assign("$(SAMPLE)/temp/copy2.pdf");
    uxp::sys::processStandardMacros(buffer);

    f1 = fopen(buffer.data(), "wb");

    while (data.readVirtualFile(handle, outbuf))
    {
      fwrite(outbuf.data(), static_cast<size_t>(outbuf.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    if (data.compareExternalFile("data.pdf", "$(SAMPLE)/temp/copy2.pdf"))
      std::cout << "Comparison of data.pdf to copy2.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy2.pdf: failed" << std::endl;

    data.close();

    /* Close the SSO session */

    std::cout << std::endl << "Sample SSO session closing" << std::endl;

    sampleid.closeSession();

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
 * @param prefix Response prefix
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxp::challenge &ch, const std::string &prefix)
{
  std::string value;
  uxl::string prompt;

  prompt = ch.getPrompt();

  ch.startTimer();

  if (prompt == "Username")
  {
    value = "SampleUser@myemail.com";
  }
  else
  {
    value = prefix + " " + prompt.mid(10);
  }

  ch.endTimer();
  ch.setValue(value);

  return 1;
}
