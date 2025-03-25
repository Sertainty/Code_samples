/*!
    \copyright Sertainty Corporation, 2020. All Rights Reserved.

    \file     id_session.cs

    \brief    This sample demonstrates how to authenticate into an ID session
              and use it to auto-authenticate, also known as, Single Sign-On,
              into a UXP Object.

    \details  A UXP Object requires authentication of the prospective user
              (process or person). Access will only be granted after a
              successful authentication.
    
              Having to individually authenticate into multiple UXP Objects is
              time-consuming. Also time-consuming is  authenticating into a
              single UXP Object multiple times. As a convenience, a UXP Identity
              can be used for a Single Sign-On session. A Single Sign-On
              session allows automatic authentication into UXP Objects that
              were created using that same UXP Identity.
    
              For authentication, there are two approaches to programatically
              seeking authorization. The first approach is to declare a function
              callback that is called when the system presents challenges.
              The callback function is given the list of challenges that it must
              process and return. The callback function is called until resolution
              is reached.
              
              The second uses a looping process to allow the program to handle the
              challenge list manually. Responses are then given back to the system
              and the authentication loop continues until a resolution is reached.

    \author   Quinn Carter
    \date     7/13/2020

    \note     Application expects to find the necessary source files in the
              current working folder.
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;

  class IdSession
  {
    static void Main(string[] args)
    {

      Console.WriteLine("Sample SSO using C#\n");

      /* Allocate a new buffer. Automatically zeros it. */

      ByteArray buffer = new ByteArray();
      string uxpFileSpec = "sample.uxp";
      string idFileSpec = "sampleid.iic";
      string dataPdfSpec = "data.pdf";
      string dataPdf2Spec = "data2.pdf";
      string copy1Spec = "copy1.pdf";
      string copy2Spec = "copy2.pdf";

      Sys.CallStatus callStatus = new Sys.CallStatus();

      /* Set up log file and initialize library.  Must do this before any active calls. */
      
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

        /* Now, open the first virtual file and write it out to a temporary file */

        VirtualFile virtualFile = uxpFile.OpenVirtualFile("data.pdf", Mode.ReadOnly);

        Console.Write("\nReading data.pdf in loop ... ");
        FileStream sw = new FileStream(copy1Spec, FileMode.Create);

        while (virtualFile.Read(buffer, 1000) > 0)
        {
          int len = (int)buffer.Size;
          byte[] data = buffer.GetBytes(len);
          sw.Write(data, 0, len);
        }

        sw.Close();
        virtualFile.Close();

        Console.WriteLine("finished reading data.pdf");

        if (uxpFile.CompareExternalFle("data.pdf", copy1Spec))
        {
          Console.WriteLine("Comparison of data.pdf to copy1.pdf: successful");
        }
        else
        {
          Console.WriteLine("Comparison of data.pdf to copy1.pdf: failed");
        }

        uxpFile.Close();

        /* Reopen the UXP Object ... includes authentication */

        Id idHandle = new Id();

        idHandle.OpenSessionFromFile(idFileSpec,false);
        Console.WriteLine("Opening ID session");

        Console.WriteLine("\nCredentials necessary to access UXP:");
        Console.WriteLine("  Username = SampleUser@myemail.com");
        Console.WriteLine("  Challenge 1 = Response 1");
        Console.WriteLine("  Challenge 2 = Response 2");
        Console.WriteLine("  ... ");
        Console.WriteLine("  Challenge 10 = Response 10\n");

        bool authorized = false;
        int status = 0;

        while (status != (int)AuthorizationStatus.Authorized)
        {

          status = idHandle.Authenticate();

          switch (status)
          {
            case (int)AuthorizationStatus.Authorized:
              Console.WriteLine("\nSSO session authorized");
              authorized = true;
              break;

            case (int)AuthorizationStatus.NotAuthorized:
              Console.WriteLine("\nSSO session not authorized");
              authorized = false;
              break;

            case (int)AuthorizationStatus.Challenged:
              for (int i = 0; i < idHandle.GetChallengeCount(); i++)
              {
                Challenge ch = idHandle.GetChallenge(i);
                getResponse(ch);

                idHandle.AddResponse(ch);
              }
              break;

            default:
              break;
          }
        }

        /* Reopen the data ... includes authentication. */

        if (authorized == true)
        {
          Console.WriteLine("Opening new Data using SSO");
          Console.WriteLine("\nExtracting data.pdf to copy2.pdf");

          int status2 = idHandle.OpenUxpFromFile(uxpFile,uxpFileSpec,1);
  
          uxpFile.ExportVirtualFile("data.pdf", "copy2.pdf", 3);

          if (uxpFile.CompareExternalFle("data2.pdf", copy2Spec))
          {
            Console.WriteLine("Comparison of data2.pdf to copy2.pdf: successful");
          }
          else
          {
            Console.WriteLine("Comparison of data2.pdf to copy2.pdf: failed");
          }

          /* Close the SSO session */
        
          idHandle.CloseSession();

          /* Close up again and free handle */
          
          uxpFile.Close();

          Console.WriteLine("\nYou successfully authenticated into an Id Session and used it to auto-authenticate into a UXP.");
          Console.WriteLine("You may try out other advanced samples now.");
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
