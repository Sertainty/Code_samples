/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file    open_uxp_auth.cs

    \brief   This sample shows how to open and interactively authenticate a
             UXP Object using known prompt/response pairs and read contents
             of virtual files protected within a UXP Object.

    \author  Quinn Carter
    \date    07/13/2020

    \note    Application expects to find the necessary source files in the current working folder.
 */


using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;

  class OpenUxpAuth
  {
    static void Main(string[] args)
    {

      Console.WriteLine("Opening and Authenticating into a UXP interactively");

      /* Allocate a new buffer. Automatically zeros it. */

      ByteArray buffer = new ByteArray();
      string uxpFileSpec = "sample.uxp";
      string copy2Spec = "copy2.pdf";

      Sys.CallStatus callStatus = new Sys.CallStatus();

      /* Set up log file and initialize library. Must do this before any active calls. */

      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");

      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
        Console.WriteLine("Sample failed. Ensure you have a valid installation and license.");
      }
      else
      {
        /* Reopen the Data ... includes authentication */

        UxpFile uxpFile = new UxpFile();

        uxpFile.Open(uxpFileSpec, UxpMode.ShareAll);

        Console.WriteLine("\nCredentials necessary to access UXP:");
        Console.WriteLine("  Username = SampleUser@myemail.com");
        Console.WriteLine("  Challenge 1 = Response 1");
        Console.WriteLine("  Challenge 2 = Response 2");
        Console.WriteLine("  ... ");
        Console.WriteLine("  Challenge 10 = Response 10\n");

        bool done = false;
        bool authorized = true;

        /* Continue to authenticate until a final decision is reached. */

        while (!done)
        {
          AuthorizationStatus status = uxpFile.Authenticate(false);

          switch (status)
          {
            case AuthorizationStatus.Authorized:
              Console.WriteLine(" - You are authorized");
              done = true;
              authorized = true;
              break;

            case AuthorizationStatus.NotAuthorized:
              Console.WriteLine(" - You are not authorized");
              authorized = false;
              done = true;
              break;

            case AuthorizationStatus.Challenged:
              for (int i = 0; i < uxpFile.ChallengeCount; i++)
              {
                Challenge ch = uxpFile.GetChallenge(i);
                getResponse(ch);

                uxpFile.AddResponse(ch);
              }
              break;

            default:
              break;
          }
        }

        /* Reopen the Data ... includes authentication. */
        
        if (authorized == true)
        {
          Console.WriteLine("\nOpening virtual file");

          VirtualFile virtualFile = uxpFile.OpenVirtualFile("data.pdf", Mode.ReadOnly);

          FileStream sw = new FileStream(copy2Spec, FileMode.Create);

          while (virtualFile.Read(buffer, 1000) > 0)
          {
            int len = (int)buffer.Size;
            byte[] data = buffer.GetBytes(len);
            sw.Write(data, 0, len);
          }
          sw.Close();
          virtualFile.Close();

          if (uxpFile.CompareExternalFle("data.pdf", copy2Spec))
          {
            Console.WriteLine("Comparison of data.pdf to copy2.pdf: successful");
          }
          else
          {
            Console.WriteLine("Comparison of data.pdf to copy2.pdf: failed");
          }

          /* Close up again and free handle */

          uxpFile.Close();

          Console.WriteLine("You have successfully opened and interactively authenticated into a UXP.");
          Console.WriteLine("You may try out other advanced samples now.");
        }
        else
        {
          Console.WriteLine("You are not authorized");
        }
      }
      Console.WriteLine("\nSample finished running");

    }

    private static void getResponse(IntPtr ch_handle)
    {
      ByteArray byteArray;
      Challenge ch;
      ch = Challenge.fromHandle(ch_handle);

      byteArray = ch.GetPrompt();
      ch.StartTimer();

      if (byteArray.ToString() == "Username")
      {
        ch.SetValue("SampleUser@myemail.com");
      }
      else
      {
        string challenge = byteArray.ToString();
        string[] list = challenge.Split(' ');
        string response = "Response " + list[1].ToString();
        ch.SetValue(response);
      }

      ch.EndTimer();
    }
  }
}
