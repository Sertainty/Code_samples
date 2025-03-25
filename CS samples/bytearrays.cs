/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file      bytearrays.cs

    \brief     This sample demonstrates the use of uxlByteArray,
               which is a UXP Data Structure that is used to handle binary and ascii data.
               Demonstrated in the sample are writing data to and reading data from the
               uxlByteArray as well as loading contents of a file read from the
               Virtual File system into the uxlByteArray.

    \author    Quinn Carter
    \date      06/29/2020

    \note
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;

  class Bytearrays
  {
    static void Main(string[] args)
    {
      string fileSpec = "temp.txt";

      Console.WriteLine("\nUXL Byte Arrays\n");

      /* Allocate a new buffer.  Automatically zeros it. */

      ByteArray buffer = new ByteArray();

      Sys.CallStatus callStatus = new Sys.CallStatus();

      string chPtrForFile = "ASCII Content for file. This can be binary as well.";
      int lPtrForFile = chPtrForFile.Length; 

      buffer.SetData(chPtrForFile);

      Console.WriteLine("String Size: " + lPtrForFile);
      Console.WriteLine("Buffer Size: " + buffer.Size);

      Console.WriteLine("String Contents: " + chPtrForFile);
      Console.WriteLine("Buffer Contents: " + buffer.ToString());

      Sys.FileWriteAll(callStatus,fileSpec,buffer, Modifiers.Replace);

      if (callStatus.HasError)
      {
        Console.WriteLine("Error writing buffer to file: {0}", callStatus.ErrorMessage);
      }
      else
      {
        Console.WriteLine("\nFile written to disk sucessfully");
        //buffer.Clear();

        Sys.FileReadAll(callStatus, fileSpec, buffer);
        if (callStatus.HasError)
        {
          Console.WriteLine("Error reading file into buffer: {0}", callStatus.ErrorMessage);
        }
        else
        {
          Console.WriteLine("Content Size: " + lPtrForFile);
          Console.WriteLine("Buffer Size: " + buffer.Size);

          Console.WriteLine("Contents: " + chPtrForFile);
          Console.WriteLine("Buffer Contetnt: " + buffer.ToString());
        }
      }
    }
  }
}
