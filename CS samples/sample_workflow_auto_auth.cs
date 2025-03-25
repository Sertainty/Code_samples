/*!
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.
   
    \file     sample_workflow_auto_auth.cs

    \brief    Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
                (1) create ID from XML
                (2) Create UXP from ID & adds data
                (3) Closes the UXP
                (4) Auto authenticates success / fail
                (5) on success, extracts the data
                (6) closes UXP 

    \edited   Melani Smith Weed 
    \date     27-jul-2020
*/

using System;

namespace Sertainty
{
  class SampleAuto
  {
    static void Main (string[] args)
    {
      Console.WriteLine("Sample C# Data Application using ID");
      
      string idXmlSpec = "sample_auto_auth_id.xml";
      string idFileSpec = "sample_auto_auth_id.iic";
      string uxpFileSpec = "sample_auto_auth.uxp";
      string dataPdfSpec = "data.pdf";
      
      UxpFile appHandle;
      
      /* Allocate a new buffer. Automatically zeros it. */
      ByteArray buffer = new ByteArray();

      Sys.CallStatus callstatus = new Sys.CallStatus();

      /* Set up log file and initialize library.  Must do this before any active calls. */
      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample AutoOpen C#", "Sample AutoOpen C# 2.0.0");
      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
      }
      else
      {
        Console.WriteLine("Library Initialized");

        Console.WriteLine(
          "Files of interest:\n" +
          "  sample_auto_auth_id.xml     - Sample ID source documnt.  IIC is created from this.\n" +
          "  sample_workflow_auto_auth.cs          - Sample source file for C#\n" +
          "  sample_auto_auth_text.xml   - Secure text source document\n" +
          "  sample_auto_auth_text.c     - Secure text C file ... auto-generated\n" +
          "  sample_auto_auth_text.h     - Secure text header file ... auto-generated\n" +
          "  sample_auto_auth_text.msg   - Secure text ... auto-generated ... must be in Sertainty home\n\n" +

          "Build notes:\n" +
          "  Compile secure text using SertaintyString utility.\n" +
          "  Compile sample_workflow_auto_auth.cs\n" +
          "  Compile sample_auto_auth_text.c\n" +
          "  Link the two compiled C files along with Sertainty library.\n\n"
          );

        /* This will initialize the secure text.  We store the necessary challenge
            and responses from the secure text message file in the Sertainty home folder. */
        sampleAutoTextInit();

        /* Create a new ID from the sample_auto_auth_id.xml ID definition.
         
           Note: an ID can be created one time and reused many times.  This
           sample is just a demonstration of the complete workflow.
        */

        Sys.FileReadAll(callstatus, idXmlSpec, buffer);
        
        if (callstatus.HasError)
        {
          Console.WriteLine("Error reading ID: {0}", callstatus.ErrorMessage);
        }
        else
        {
          Console.WriteLine("{0} read", idXmlSpec);
          string doc = buffer.ToString();
          Id.PublishIdToFile(callstatus, idFileSpec, doc, (long)Modifiers.Replace);
          
          if (callstatus.HasError)
          {
            Console.WriteLine("Error creating ID: {0}", callstatus.ErrorMessage);
          }
          else
          {
            Console.WriteLine("{0} created", idFileSpec);

            /* Create new UXP from ID */

            Console.WriteLine("Creating new UXP: {0}", uxpFileSpec);
            Console.WriteLine("Protecting document data.pdf");

            appHandle = new UxpFile();
            appHandle.OpenNewFile(uxpFileSpec, idFileSpec, 3, (long)Modifiers.Replace, 0);
            
            if (appHandle.HasError)
            {
              Console.WriteLine("Error creating Data: {0}", appHandle.ErrorMessage);
            }
            else
            {
              Console.WriteLine("{0} created", uxpFileSpec);

              /* Create a new virtual file inside the Data*/
              appHandle.AddVirtualFile("data.pdf", dataPdfSpec, -1, -1, (long)Modifiers.Compress);
              
              if (appHandle.HasError)
              {
                Console.WriteLine("Error creating virtual file: {0}", appHandle.ErrorMessage);
              }
              else
              {
                Console.WriteLine("{0} added", dataPdfSpec);

                //Close the UXP. This will delete the handle as well
                    
                appHandle.Close();

                /* Reopen the UXP ... includes auto authentication. */

                appHandle.Open(uxpFileSpec, UxpMode.ShareAll);
                    
                if (appHandle.HasError)
                {
                  Console.WriteLine("Error opening UXP: {0}", appHandle.ErrorMessage);
                }
                else
                {
                  Console.WriteLine("Opening new UXP and auto-authenticating");

                  string username = UxlMsg.GetSecureText(callstatus, "Sample", (int)Sample_T_Id.SAMPLE_T_USER);

                  Challenge ch = new Challenge();
                  ch.SetName("USERNAME");
                  ch.SetPrompt("Username");
                  ch.SetValue(username);

                  appHandle.AddResponse(ch);

                  for(int i = 0; i < 10; i++)
                  {
                    string challenge = UxlMsg.GetSecureText(callstatus, "Sample", (int)Sample_T_Id.SAMPLE_T_CH0 + i);
                    string response = UxlMsg.GetSecureText(callstatus, "Sample", (int)Sample_T_Id.SAMPLE_T_RS0 + i);

                    ch.SetName(challenge);
                    ch.SetPrompt(challenge);
                    ch.SetValue(response);

                    appHandle.AddResponse(ch);
                    
                  }

                  AuthorizationStatus status;


                  status = appHandle.Authenticate(false);

                  bool authorized;
                  switch (status)
                  {
                    case AuthorizationStatus.Authorized:
                      Console.WriteLine("Access has been authorized");
                      authorized = true;
                      break;

                    default:
                    case AuthorizationStatus.NotAuthorized:
                      Console.WriteLine("Access denied");
                      authorized = false;
                      break;
                  }


                  if (authorized)
                  {
                    Console.WriteLine("Extracting data.pdf to copy.pdf");

                    appHandle.ExportVirtualFile("data.pdf", "copy.pdf", (long)Modifiers.Replace);
                  }
                }
              }
            }
            
            appHandle.Close();
          }
          
          Console.WriteLine("Sample finished running. Press any key to exit...");
          
          Console.ReadKey();
        }
      }
    }

    /* from the generated sample_auto_auth_text.h header file */
    private enum Sample_T_Id : int
    {
      SAMPLE_T_CH0 = 1,
      SAMPLE_T_CH1 = 2,
      SAMPLE_T_CH2 = 3,
      SAMPLE_T_CH3 = 4,
      SAMPLE_T_CH4 = 5,
      SAMPLE_T_CH5 = 6,
      SAMPLE_T_CH6 = 7,
      SAMPLE_T_CH7 = 8,
      SAMPLE_T_CH8 = 9,
      SAMPLE_T_CH9 = 10,
      SAMPLE_T_RS0 = 11,
      SAMPLE_T_RS1 = 12,
      SAMPLE_T_RS2 = 13,
      SAMPLE_T_RS3 = 14,
      SAMPLE_T_RS4 = 15,
      SAMPLE_T_RS5 = 16,
      SAMPLE_T_RS6 = 17,
      SAMPLE_T_RS7 = 18,
      SAMPLE_T_RS8 = 19,
      SAMPLE_T_RS9 = 20,
      SAMPLE_T_USER = 21
    };

    /* from the generated sample_auto_auth_text.c code file */
    private static void sampleAutoTextInit()
    {
      //int key_size = 216; //Not needed in C#. Can be determined by the Length property of the declared byte[] key.
      byte[] key =  {
                53,104,79,67,106,55,112,104,118,78,68,90,47,82,84,66,67,121,77,89,89,
                86,100,112,109,85,82,114,82,72,111,98,75,48,69,76,103,73,105,67,84,106,
                101,53,113,105,101,120,99,83,121,114,122,55,103,102,89,119,51,90,78,104,99,
                48,114,105,119,97,102,101,79,73,82,70,119,116,99,104,52,65,69,97,43,56,
                75,74,97,103,111,113,88,83,76,66,114,76,79,111,78,71,68,112,72,71,103,
                66,107,54,102,76,54,83,118,112,50,48,70,50,48,73,68,51,105,107,51,57,
                117,70,103,55,86,119,104,108,105,101,50,81,116,115,118,74,82,107,75,57,113,
                69,99,70,70,115,73,48,54,108,68,80,57,99,43,80,112,87,57,106,51,103,
                102,79,78,89,53,81,72,84,87,69,118,86,102,118,100,47,103,74,114,79,109,
                79,48,111,85,100,120,111,71,119,48,78,71,81,49,78,98,81,49,43,97,82,
                111,79,68,82,49,53
               };

      Sys.CallStatus status = new Sys.CallStatus();

      UxlMsg.LoadSecureString(status, "Sample", "sample_auto_auth_text.msg", key);

    }
  }
}
