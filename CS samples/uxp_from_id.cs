/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file    uxp_from_id.cs

    \brief   This sample demonstrates how to create a UXP Object using a UXP Identity
             and add data files into the created UXP Object.
    
    \author  Quinn Carter
    \date    06/19/2020

    \note    Application expects to find the necessary source files in the current working folder.
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;
   
  class UxpFromId
  {
    static void Main(string[] args)
    {
      Console.WriteLine("Creating a Sertainty ID from an ID Definition XML");

      /* Allocate a new buffer. Automatically zeros it. */
      
      ByteArray buffer = new ByteArray();
      string uxpFileSpec = "sample.uxp"; // Source ID Definition XML
      string idFileSpec = "sampleid.iic"; // Sertainty ID file name to be created
      string dataPdfSpec = "data.pdf"; // First file to be protected
      string dataPdf2Spec = "data2.pdf"; // Second file to be protected

      /* Set up log file and initialize library. Must do this before any active calls. */
      
      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");

      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
        Console.WriteLine("Sample failed. Ensure you have a valid installation and license.");
      }
      else
      {
        /* Create new UXP Object from UXP Identity */
                
        UxpFile uxpFile = new UxpFile();

        uxpFile.OpenNewFile(uxpFileSpec, idFileSpec, 3, 1, 0);

        /* Create a new virtual file inside the Data */

        Console.WriteLine("Adding data to UXP");
                
        uxpFile.AddVirtualFile("data.pdf", dataPdfSpec, -1, -1, 8);

        Console.WriteLine("Adding data to UXP");
                
        uxpFile.AddVirtualFile("data2.pdf", dataPdf2Spec, -1, -1, 8);

        Console.WriteLine("\nYou have successfully created a protected UXP and added files into it.");
        Console.WriteLine("You can now try to open the UXP to read the contents.");
        Console.WriteLine("\nSample finished running");

        uxpFile.Close();
      }
    }
  }
}
