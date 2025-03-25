/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file     id_from_xml.cs

    \brief    This sample demonstrates constructing a UXP Identity file (*.iic)
              from an ID Definition XML Source file (*.xml). The generated UXP
              Identity file (*.iic) can be used to generate a UXP Object.
              
    \author   Quinn Carter
    \date     06/19/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;

  class IdFromXml
  {
    static void Main(string[] args)
    {
      Console.WriteLine("Creating a Sertainty ID from an ID Definition XML");

      /* Allocate a new buffer. Automatically zeros it. */

      ByteArray buffer = new ByteArray();
      string idXmlSpec = "sampleid.xml"; // Source ID Definition XML
      string idFileSpec = "sampleid.iic"; // Sertainty ID file name to be created
      
      Sys.CallStatus callStatus = new Sys.CallStatus();

      /*
          Set up log file and initialize library.
          Must do this before any active calls.
      */
      
      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");

      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
        Console.WriteLine("Sample failed. Ensure you have a valid installation and license.");
      }
      else
      {
        
        Sys.FileReadAll(callStatus, idXmlSpec, buffer);
                
        if (callStatus.HasError)
        {
          
          Console.WriteLine("Error reading ID: {0}",callStatus.ErrorMessage);
        }
        else
        {
          /* Create a new UXP Identity from the sampleid.xml ID Definition XML Source */
          
          Id.PublishIdToFile(callStatus, idFileSpec, buffer.ToString(), 1);
          
          Console.WriteLine("\nYour ID is now created. Try protecting a file with it.");

          Console.WriteLine("\nSample finished running");
        }
      }
    }
  }
}
