/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_workflow_create_id_uxp_open.cs

    \brief    Demonstrates a mini workflow, resulting in automatic UXP authentication/ SSO;
                (1) create ID from XML
                (2) Create UXP from ID & adds data
                (3) Closes the UXP
                (4) Auto authenticates success / fail
                (5) on success, extracts the data
                (6) closes UXP

    \author   Karim Lalani
    \date     06/20/2018
 
    \edited   Melani Smith Weed
    \date     27-jul-2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;
  //using static MappingsCApi;
  
  class Program
  {
    static void Main(string[] args)
    {
      Console.WriteLine("Sample C# Data Application using ID\n");
      
      string idXmlSpec = "sampleid.xml";
      string idFileSpec = "sampleid.iic";
      string uxpFileSpec = "sample.uxp";
      string dataPdfSpec = "data.pdf";
      string dataPdf2Spec = "data2.pdf";
      string copy1Spec = "copy1.pdf";
      string copy2Spec = "copy2.pdf";
      
      ByteArray buffer = new ByteArray();
      Sys.CallStatus callStatus = new Sys.CallStatus();

      /* Set up log file and initialize library.  Must do this before any active calls. */

      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0"); //long ret = uxpsys_initLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");
      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
        Console.WriteLine("Sample failed. Ensure you have a valid installation and license.");
      }
      else
      {
        Console.WriteLine("Library Initialized\n");
        Sys.FileReadAll(callStatus, idXmlSpec, buffer);
        
        if (callStatus.HasError)
        {
          Console.WriteLine("Error reading ID: {0}",callStatus.ErrorMessage);
        }
        else
        {
          /* Create a new ID from the sampleid.xml or substituted xml ID definition; must update this sample with specific xml file name. */

          Console.WriteLine("{0} read", idXmlSpec);
          Id.PublishIdToFile(callStatus, idFileSpec, buffer.ToString(), 1);
          
          if (callStatus.HasError) //if (uxpsys_hasError(callstatus))
          {
            Console.WriteLine("Error creating ID: {0}",callStatus.ErrorMessage);
          }
          else
          {
            /* Create new UXP from ID */

            Console.WriteLine("{0} created", idFileSpec);

            UxpFile uxpFile = new UxpFile();
            uxpFile.OpenNewFile(uxpFileSpec, idFileSpec, 3, 1, 0);
            
            if (uxpFile.HasError)
            {
              Console.WriteLine("Error creating Data: {0}",uxpFile.ErrorMessage);
            }
            else
            {
              Console.WriteLine("{0} created\n", uxpFileSpec);

              /* Console.WriteLine("Adding data to UXP"); */

              uxpFile.AddVirtualFile("data.pdf", dataPdfSpec, -1, -1, 8);
              
              if (uxpFile.HasError)
              {
                Console.WriteLine("Error creating virtual file: {0}",uxpFile.ErrorMessage);
              }
              else
              {
                Console.WriteLine("{0} added", dataPdfSpec);

                uxpFile.AddVirtualFile("data2.pdf", dataPdf2Spec, -1, -1, 8);
                
                if (uxpFile.HasError) //if (uxpsys_hasError(appHandle))
                {
                  Console.WriteLine("Error creating virtual file: {0}",uxpFile.ErrorMessage);
                }
                else
                {
                  Console.WriteLine("{0} added\n", dataPdf2Spec);
                  
                  /* Now open the first virtual file and write it out to a temporary file. */

                  VirtualFile virtualFile = uxpFile.OpenVirtualFile("data.pdf", Mode.ReadOnly);
                  
                  if (uxpFile.HasError)
                  {
                    Console.WriteLine("Error opening virtual file: {0}",uxpFile.ErrorMessage);
                  }
                  else
                  {
                    Console.WriteLine("{0} opened", "data.pdf");
                    
                    Console.Write("Reading data.pdf in loop ... ");
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
                      Console.WriteLine("Comparison of data.pdf to copy1.pdf: successful\n");
                    }
                    else
                    {
                      Console.WriteLine("Comparison of data.pdf to copy1.pdf: failed\n");
                    }
                    
                    /* Close the UXP. This will delete the handle as well */
                    
                    uxpFile.Close();
                    
                    /* Reopen the Data ... includes authentication */
                    
                    Console.WriteLine("Re-opening Data");
                    
                    uxpFile.Open(uxpFileSpec, UxpMode.ShareAll);
                    
                    if (uxpFile.HasError)
                    {
                      Console.WriteLine("Error opening UXP: {0}",uxpFile.ErrorMessage);
                    }
                    else
                    {
                      Console.WriteLine("Credentials necessary to access UXP:");
                      Console.WriteLine("  Username = SampleUser@myemail.com");
                      Console.WriteLine("  Challenge 1 = Response 1");
                      Console.WriteLine("  Challenge 2 = Response 2");
                      Console.WriteLine("  ... ");
                      Console.WriteLine("  Challenge 10 = Response 10\n");
                      
                      bool done = false;
                      bool authorized = false;
                      
                      /* Continue to authenticate until a final decision is reached. */

                      while (!done)
                      {
                        AuthorizationStatus status = uxpFile.Authenticate(false);

                        switch (status)
                        {
                          case AuthorizationStatus.Authorized:
                            Console.WriteLine(" - You are authorized\n");
                            done = true;
                            authorized = true;
                            break;

                          case AuthorizationStatus.NotAuthorized:
                            Console.WriteLine(" - You are not authorized\n");
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
                      
                      if (authorized == true)
                      {
                        Console.WriteLine("Extracting data.pdf to copy2.pdf");

                        uxpFile.ExportVirtualFile("data.pdf", "copy2.pdf", 3);

                        if (uxpFile.CompareExternalFle("data2.pdf", copy2Spec))
                        {
                          Console.WriteLine("Comparison of data2.pdf to copy2.pdf: successful");
                        }
                        else
                        {
                          Console.WriteLine("Comparison of data2.pdf to copy2.pdf: failed");
                        }
                      }

                      uxpFile.Close();
                    }
                  }
                }
              }
            }
          }

          Console.WriteLine("\nSample finished running.");
        }
      }
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
