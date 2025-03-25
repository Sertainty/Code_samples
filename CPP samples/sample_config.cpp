/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_config.cpp

    \brief    Sample program using C language interface that demonstrates
              fetching current machine's location information using the
              Sertainty SDK. Similarly, machine's network interface information
              can also be fetched from the machine's configuration. This sample
              only demonstrates location information for brevity.

    \author   Quinn Carter
    \date     07/20/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile.h"
#include "uxpfile_common.h"
#include "uxlbytearray.h"
#include "uxpcredential.h"
#include "uxpid.h"
#include "uxpsys.h"
#include "uxpconfig.h"

#include <iostream>

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  /* Allocate a new buffer.  Automatically zeros it. */

  uxl::bytearray buffer;
  bool licensed;

  /* Set up log file and initialize library.  Must do this before any active calls. */
    
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

    std::cout << std::endl << "Sample C++ Config/Location Application" << std::endl;
    
    /* Load the current machine configuration to extract location and network information */

    uxp::config config = config.getCurrentConfig();

    /* Extract location config from machine config */

    uxp::location location = config.getLocation();

    /* Get the Zipcode from location config */

    std::cout << "\nZipcode Property from location: " << location.getProperty("Zipcode").toString() << std::endl << std::endl;

    /* Get the location config table to read location data iteratively */

    std::vector<uxl::variant> table = location.getTable();

    /* Get property count on location config table */

    int tableCount = table.size();

    for(int i = 0; i< tableCount; i++)
    {
      /* Fetch location config property row at index i */

      uxl::variant row = table[i];

      /* Get the location config property name from row */

      std::cout << row.getName() << ": ";

      /* Get the location config property value from row as string */

      std::cout << row.toString() << std::endl;
    }

    std::cout<<"\nSample finished running\n";
    return 0;
  }
  catch (uxl::exception &e2)
  {
    std::cout << "Error: " << e2.getErrorText() << std::endl;

    return 1;
  }
}
