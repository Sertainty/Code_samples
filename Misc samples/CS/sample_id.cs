using System;
using Sertainty;

namespace Sertainty
{
  using System.IO;
  
  class SampleID
  {
    static void Main (string[] args)
    {
      Console.WriteLine("Sample C# Data Application using ID");
      
      string idXmlSpec = "sampleid.xml";
      string idFileSpec = "sampleid.iic";
      string uxpFileSpec = "sample.uxp";
      string dataPdfSpec = "data.pdf";
      string dataPdf2Spec = "data2.pdf";
      string copy1Spec = "copy1.pdf";
      string copy2Spec = "copy2.pdf";
      
      UxpFile appHandle;
      
      ByteArray buffer = new ByteArray();
      Sys.CallStatus callstatus = new Sys.CallStatus();
      
      Sys.SetLogFile("Sample C#", "Sample C# 1.0");
      
      long ret = Sys.InitializeLibrary(buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");
      if (ret == 0)
      {
        Console.WriteLine("Error initializing environment: {0}", buffer);
      }
      else
      {
        Console.WriteLine("Library Initialized");
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
            appHandle = new UxpFile();
            appHandle.OpenNewFile(uxpFileSpec, idFileSpec, 3, (long)Modifiers.Replace, 0);
            
            if (appHandle.HasError)
            {
              Console.WriteLine("Error creating Data: {0}", appHandle.ErrorMessage);
            }
            else
            {
              Console.WriteLine("{0} created", uxpFileSpec);
              appHandle.AddVirtualFile("data.pdf", dataPdfSpec, -1, -1, (long)Modifiers.Compress);
              
              if (appHandle.HasError)
              {
                Console.WriteLine("Error creating virtual file: {0}", appHandle.ErrorMessage);
              }
              else
              {
                Console.WriteLine("{0} added", dataPdfSpec);
                appHandle.AddVirtualFile("data2.pdf", dataPdf2Spec, -1, -1, (long)Modifiers.Compress);
                
                if (appHandle.HasError)
                {
                  Console.WriteLine("Error creating virtual file: {0}", appHandle.ErrorMessage);
                }
                else
                {
                  Console.WriteLine("{0} added", dataPdf2Spec);
                  
                  //Now open the first virtual file and write it out to a temporary file.
                  VirtualFile fileHandle = appHandle.OpenVirtualFile("data.pdf", Mode.ReadOnly);
                  
                  if (appHandle.HasError)
                  {
                    Console.WriteLine("Error opening virtual file: {0}", appHandle.ErrorMessage);
                  }
                  else
                  {
                    Console.WriteLine("{0} opened", "data.pdf");
                    
                    Console.WriteLine("Reading data.pdf in loop ...");
                    FileStream sw = new FileStream(copy1Spec, FileMode.Create);
                    
                    while (fileHandle.Read(buffer, 1000) > 0)
                    {
                      int len = (int)buffer.Size;
                      byte[] data = buffer.GetBytes(len);
                      //long len = uxpba_getSize(buffer);
                      sw.Write(data, 0, len);
                    }
                    sw.Close();
                    fileHandle.Close();
                    Console.WriteLine("Finished reading data.pdf");
                    
                    if (appHandle.CompareExternalFle("data.pdf", copy1Spec))
                    {
                      Console.WriteLine("Comparison of data.pdf to copy1.pdf: successful");
                    }
                    else
                    {
                      Console.WriteLine("Comparison of data.pdf to copy1.pdf: failed");
                    }
                    
                    //Close the UXP. This will delete the handle as well
                    
                    appHandle.Close();
                    
                    //Reopen the Data ... includes authentication
                    
                    Console.WriteLine("Opening new Data");
                    
                    appHandle.Open(uxpFileSpec, UxpMode.ShareAll);
                    
                    if (appHandle.HasError)
                    {
                      Console.WriteLine("Error opening UXP: {0}", appHandle.ErrorMessage);
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
                      AuthorizationStatus status;
                      
                      while (!done)
                      {
                        status = appHandle.Authenticate(false);
                        
                        switch (status)
                        {
                          case AuthorizationStatus.Authorized:
                            Console.WriteLine("You're authorized");
                            done = true;
                            authorized = true;
                            break;
                            
                          case AuthorizationStatus.NotAuthorized:
                            Console.WriteLine("You're not authorized");
                            authorized = false;
                            done = true;
                            break;
                            
                          case AuthorizationStatus.Challenged:
                            for (int i = 0; i < appHandle.ChallengeCount; i++)
                            {
                              Challenge ch = appHandle.GetChallenge(i);
                              getResponse(ch);
                              appHandle.AddResponse(ch);
                            }
                            break;
                          
                          default:
                            break;
                        }
                      }
                      
                      if (authorized)
                      {
                        Console.WriteLine("Extracting data.pdf to copy2.pdf");
                        
                        fileHandle = appHandle.OpenVirtualFile("data.pdf", Mode.ReadOnly);
                        
                        if (appHandle.HasError)
                        {
                          Console.WriteLine("Error opening virtual file: {0}",  appHandle.ErrorMessage);
                        }
                        else
                        {
                          sw = new FileStream(copy2Spec, FileMode.Create);
                          while (fileHandle.Read(buffer, 1000) > 0)
                          {
                            int len = (int)buffer.Size;
                            byte[] data = buffer.GetBytes(len);
                            sw.Write(data, 0, len);
                          }
                          sw.Close();
                          fileHandle.Close();
                        }
                      }
                    }
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
