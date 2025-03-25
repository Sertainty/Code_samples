/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file    id_session.cpp

    \brief   This sample demonstrates how to authenticate into an ID session and
             use it to auto-authenticate, also known as, Single Sign-On, into a UXP Object.
 
    \details A UXP Object requires authentication of the prospective user
             (process or person). Access will only be granted after a
             successful authentication.
    
             Having to individually authenticate into multiple UXP Objects is time-consuming.
             Also time-consuming is  authenticating into a single UXP Object multiple times.
             As a convenience, a UXP Identity can be used for a Single Sign-On session.
             A Single Sign-On session allows automatic authentication into UXP Objects
             that were created using that same UXP Identity.
    
             For authentication, there are two approaches to programatically seeking
             authorization. The first approach is to declare a function callback that
             is called when the system presents challenges. The callback function is
             given the list of challenges that it must process and return. The callback
             function is called until resolution is reached.

             The second uses a looping process to allow the program to handle the
             challenge list manually. Responses are then given back to the system and
             the authentication loop continues until a resolution is reached.
 
    \author  Quinn Carter
    \date    07/03/2020

    \note    Application expects to find the necessary source files in the current working folder.
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
 * @param ch Challenge object.
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
  std::string outfile("$(SAMPLE)/temp/sample.uxp");
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  const char *dataPdfSpec = "$(SAMPLE)/SampleData/data.pdf";
  const char *dataPdf2Spec = "$(SAMPLE)/SampleData/data2.pdf";
  const char *copy1Spec = "$(SAMPLE)/temp/copy1.pdf";
  const char *copy2Spec = "$(SAMPLE)/temp/copy2.pdf";
  int status = StatusChallenged;

  std::cout << std::endl << "Sample SSO using C++" << std::endl << std::endl;

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
    data.openNewFile(outfile, idFileSpec, IdFile, ModifierReplace, 0);

    /* Create a new virtual file inside the Data */

    data.addVirtualFileFromFile("data.pdf", dataPdfSpec, -1, -1, 0);
    std::cout << "Adding data to UXP" << std::endl;

    data.addVirtualFileFromFile("data2.pdf", dataPdf2Spec, -1, -1, 0);
    std::cout << "Adding data to UXP" << std::endl;

    /* Now, open the first virtual file and write it out to a temporary file */

    uxp::openVFHandle *handle = data.openVirtualFile("data.pdf", ModeRead);

    std::cout << std::endl << "Reading data.pdf in loop ... ";

    buffer.assign(copy1Spec);
    uxp::sys::processStandardMacros(buffer);

    FILE *f1 = fopen(buffer.data(), "wb");

    while (data.readVirtualFile(handle, buffer))
    {
      fwrite(buffer.data(), static_cast<size_t>(buffer.size()), 1, f1);
    }

    fclose(f1);
    data.closeVirtualFile(handle);

    std::cout << "finished reading data.pdf" << std::endl;
    
    /* Compare the extracted file with the copy inside the Data */

    if (data.compareExternalFile("data.pdf", copy1Spec))
      std::cout << "Comparison of data.pdf to copy1.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data.pdf to copy1.pdf: failed" << std::endl;

    /* Close the UXP Object. This will delete the handle as well. */

    data.close();

    std::cout << "Opening ID for SSO session" << std::endl << std::endl;

    std::cout << "Credentials necessary to access UXP:" << std::endl;
    std::cout << "  Username = SampleUser@myemail.com" << std::endl;
    std::cout << "  Challenge 1 = Response 1" << std::endl;
    std::cout << "  Challenge 2 = Response 2" << std::endl;
    std::cout << "  ... " << std::endl;
    std::cout << "  Challenge 10 = Response 10" << std::endl << std::endl;
    
    uxp::id idhandle;

    idhandle.openSessionFile(idFileSpec);
    
    while (status != StatusAuthorized)
    {
      status = idhandle.authenticate();

      switch (status)
      {
        case StatusAuthorized:
          std::cout << std::endl << "SSO session authorized" << std::endl;
          break;

        case StatusNotAuthorized:
          std::cout << std::endl << "SSO session not authorized" << std::endl;
          return 1;

        case StatusChallenged:
          for (uxp::challenge ch: idhandle.getChallenges())
          {

            // status = getResponse(ch);
            if (!getResponse(ch))
            {
              std::cout << "SSO session authorization canceled" << std::endl;

              return 1;
            }

            idhandle.addResponse(ch);
          }
          break;

        default:
          break;
      }
    }

    /* Reopen the UXP Object ... includes authentication */

    std::cout << "Opening UXP from session" << std::endl;

    status = idhandle.openUxp(data,outfile,ShareReadOnly);

    std::cout << std::endl << "Extracting data2.pdf to copy2.pdf" << std::endl;

    data.exportVirtualFile("data2.pdf", copy2Spec, ModeRead);
  
    if (data.compareExternalFile("data2.pdf", copy2Spec))
      std::cout << "Comparison of data2.pdf to copy2.pdf: successful" << std::endl;
    else
      std::cout << "Comparison of data2.pdf to copy2.pdf: failed" << std::endl;

    /* Close up again and free handle */

    data.close();
    
    /* Close the SSO session */

    idhandle.closeSession();

    std::cout << std::endl << "You successfully authenticated into an Id Session and used it to auto-authenticate into a UXP." << std::endl;
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
