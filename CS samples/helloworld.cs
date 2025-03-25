/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     helloworld.cs

    \brief    This sample demonstrates the initialize library call
              necessary before any UXP Identity or UXP Object functions can be called.

    \author   Quinn Carter
    \date     06/17/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;
  

  class Helloworld
  {
    static void Main(string[] args)
    {
      Console.WriteLine("Starting Sample");

      /* Allocate a new buffer. Automatically zeros it. */

      ByteArray buffer = new ByteArray();

      /* Set up log file and initialize library.  Must do this before any active calls. */

      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");
      
      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
        Console.WriteLine("Sample failed. Ensure you have a valid installation and license.");
      }
      else
      {
        Console.WriteLine("\nHello, Sertainty!!");
        Console.WriteLine("\nYou have a valid license. Try out other samples.");
      }
    }
  }
}
