/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_config.cs

    \brief    Sample program using C language interface that demonstrates
              fetching current machine's location information using the
              Sertainty SDK. Similarly, machine's network interface information
              can also be fetched from the machine's configuration. This sample
              only demonstrates location information for brevity.

    \author   Quinn Carter
    \date     07/20/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */
 
using System;

namespace Sertainty
{
  using System.IO;
  
  class SampleConfig
  {
    static void Main(string[] args)
    {
      Console.WriteLine("Sample C# Config/Location Application");
      
      /* Allocate a new buffer.  Automatically zeros it. */

      ByteArray buffer = new ByteArray();

      Sys.CallStatus callstatus = new Sys.CallStatus();
      
      Sys.SetLogFile("Sample C#", "Sample C# 1.0");
      
      /* Set up log file and initialize library.  Must do this before any active calls. */

      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");
      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
      }
      else
      {        
        /* Load the current machine configuration to extract location and network information */

        Config config = Config.GetCurrentConfig(false);

        if (config.HasError)
        {
          Console.WriteLine("Error Getting Current Config: {0}", config.ErrorMessage);
        }
        else
        {
          /* Extract location config from machine config */

          Location location = config.GetLocation();

          if (location.HasError)
          {
            Console.WriteLine("Error Getting Location: {0}", location.ErrorMessage);
          }
          else
          {
            /* Get the Zipcode from location config */
            
            string zipcode = location.GetProperty("Zipcode");
            Console.WriteLine("\nZipcode Property from Location: {0}\n", zipcode);

            /* Get the location config table to read location data iteratively */

            VariableList table = location.GetTable();

            /* Get property count on location config table */

            int tableCount = table.Count;

            for (int i = 0; i < tableCount; i++)
            {
              /* Fetch location config property row at index i */

              Variable row = table.GetItem(i);

              /* Get the location config property value from row as string */

              Console.WriteLine("{0,-20}: {1}",row.Name,row.ToString());
            }
          }
        }
        Console.WriteLine("\nSample finished running.");
      }
    }
  }
}
