/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_sm_exchange.cs

    \brief    Sample program using CS language interface and SMEX. This samples demonstrates how to create a Smart Message Exchange between two ID's.
              (1) Two on-demand, one-time temporary ID's are created, they will each be used to Encode and Decode messages (ID1.iic and ID2.iic)
              (2) Two SMEX sessions are initialized
              (3) Two tests are performed, one after the other:
                   - For the first test: ID1.iic will encode a smart message and ID2.iic will decode the smart message.
                   - For the second test: ID2.iic will encode a smart message and ID1.iic will decode the smart message.

    \note     Application expects to find the necessary source files in the current working folder.
 */

using System;
using Sertainty;

namespace ConsoleTest
{
    class SampleSMID
    {
        public static void Main(string [] args)
        {
            string msgFileSpec = "msg.file";

            ByteArray buffer = new ByteArray ();
            Sys.CallStatus callstatus = new Sys.CallStatus ();

            Sys.SetLogFile ("Sample C#", "Sample C# 1.0");

            long ret = Sys.InitializeLibrary (buffer, args.LongLength, args, "sertainty.lic", "SertintyONE", "Sample C#", "Sample C# 1.0");

            if (ret == 0) 
            {
                Console.WriteLine ("Error initializing environment: {0}", buffer);
            } 
            else 
            {
                bool result = false;
                SMEX smex1 = new SMEX ();
                SMEX smex2 = new SMEX ();

                string srcId1Spec = "ID1.iic"; 
                string srcId2Spec = "ID2.iic";

                /* Initialize First SMEX Session */

                Console.WriteLine ("Initializing SMEX1 Session ...");
                buffer.Clear ();
                if (result = smex1.Initialize (buffer))
                {
                    Modifiers mods = 0;
                    mods = Modifiers.Replace;

                    Sys.FileWriteAll (callstatus, srcId1Spec, buffer, mods);
                    if (callstatus.HasError)
                    {
                        Console.WriteLine ("Error writing file {0}", srcId1Spec);
                    }
                    else
                    {
                        Console.WriteLine ("{0} created successfully", srcId1Spec);
                        Console.WriteLine ("Buffer Size: {0}", buffer.Size);
                    }
                }
                else
                {
                    Console.WriteLine ("SMEX1 Initialization Failed");
                }

                /* Initialize Second SMEX Session */

                Console.WriteLine ("\nInitializing SMEX2 Session ...");
                buffer.Clear ();
                if (result = smex2.Initialize (buffer))
                {
                    Modifiers mods = 0;
                    mods = Modifiers.Replace;

                    Sys.FileWriteAll (callstatus, srcId2Spec, buffer, mods);
                    if (callstatus.HasError)
                    {
                        Console.WriteLine ("Error writing file {0}", srcId2Spec);
                    }
                    else
                    {
                        Console.WriteLine ("{0} created successfully", srcId2Spec);
                        Console.WriteLine ("Buffer Size: {0}", buffer.Size);
                    }
                }
                else
                {
                    Console.WriteLine ("SMEX2 Initialization Failed");
                }
                
                if (!result)
                {
                    Console.WriteLine ("Error initializing SMEX token");
                }
                else
                {
                    Console.WriteLine ("\nSMEX token initialized\n");
                    string destId1Spec = "ID1.iic";
                    string destId2Spec = "ID2.iic";

                    /* TEST 1: Encode using 'ID1.iic', Decode using 'ID2.iic' */

                    buffer.Clear ();
                    Sys.FileReadAll (callstatus, destId2Spec, buffer);
                    if (callstatus.HasError)
                    {
                        Console.WriteLine ("Error reading ID: {0}", callstatus.ErrorMessage);
                        result = false;
                    }
                    else
                    {
                        Console.WriteLine ("Setting 'ID2' as destination file ...");
                        Console.WriteLine ("Buffer Length: {0}", buffer.Size);
                        result = smex1.StartSession (buffer);
                        Console.WriteLine ("StartSession: {0}", result);
                    }

                    if (!result)
                    {
                        Console.WriteLine ("Error starting SMEX1 Session");
                    }
                    else
                    {
                        Console.WriteLine ("SMEX1 session started");
                        buffer.Clear ();

                        /* Encoding Message using ID1.iic */

                        Console.WriteLine("\nEncoding Message using ID1.iic ...");
                        string msg = "Hello ID2.iic!";

                        if (!smex1.Encode (msg, buffer))
                        {
                            Console.WriteLine ("Failed to Encode Message");
                            return;
                        }
                        else
                        {
                            Console.WriteLine ("Message Encoded Successfully");
                            Sys.FileWriteAll (callstatus, msgFileSpec, buffer, Modifiers.Replace);
                            if (callstatus.HasError)
                            {
                                Console.WriteLine ("Error Writing Encoded Message: {0}", callstatus.ErrorMessage);
                                return;
                            }
                            Console.WriteLine ("Encoded message is written to {0}", msgFileSpec);
                        }
                    
                        Console.WriteLine("\nDecoding Message using ID2.iic ...");
                        buffer.Clear ();
                        Sys.FileReadAll (callstatus, msgFileSpec, buffer);
                        if (callstatus.HasError)
                        {
                            Console.WriteLine ("Error Reading Encoded Message: {0}", callstatus.ErrorMessage);
                            return;
                        }
                        Console.WriteLine ("Encoded message read from {0}", msgFileSpec);
                        string encoded = buffer.ToString ();

                        /* Decoding Message using ID2.iic */

                        buffer.Clear ();
                        if (!smex2.Decode (encoded, buffer))
                        {
                            Console.WriteLine ("Failed to Decode Message");
                            return;
                        }
                        else
                        {
                            Console.WriteLine ("Decoded Message: \n{0}\n", buffer.ToString ());
                        }
                    }

                    /* TEST 2: Encode using 'ID2.iic', Decode using 'ID1.iic' */

                    buffer.Clear ();
                    Sys.FileReadAll (callstatus, destId1Spec, buffer);
                    if (callstatus.HasError)
                    {
                        Console.WriteLine ("Error reading ID: {0}", callstatus.ErrorMessage);
                        result = false;
                    }
                    else
                    {
                        Console.WriteLine ("Setting 'ID1' as destination file ...");
                        Console.WriteLine ("Buffer Length: {0}", buffer.Size);
                        result = smex2.StartSession (buffer);
                        Console.WriteLine ("StartSession: {0}", result);
                    }

                    if (!result)
                    {
                        Console.WriteLine ("Error starting SMEX2 Session");
                    }
                    else
                    {
                        Console.WriteLine ("SMEX2 session started");
                        buffer.Clear ();

                        /* Encoding Message using ID2.iic */

                        Console.WriteLine("\nEncoding Message using ID2.iic ...");
                        string msg = "Hi ID1.iic, I received your message!";

                        buffer.Clear ();
                        if (!smex2.Encode (msg, buffer))
                        {
                            Console.WriteLine ("Failed to Encode Message");
                            return;
                        }
                        else
                        {
                            Console.WriteLine ("Message Encoded Successfully");
                            Sys.FileWriteAll (callstatus, msgFileSpec, buffer, Modifiers.Replace);
                            if (callstatus.HasError)
                            {
                                Console.WriteLine ("Error Writing Encoded Message: {0}", callstatus.ErrorMessage);
                                return;
                            }
                            Console.WriteLine ("Encoded message is written to {0}", msgFileSpec);
                        }
                    
                        Console.WriteLine("\nDecoding Message using ID1.iic ...");
                        buffer.Clear ();
                        Sys.FileReadAll (callstatus, msgFileSpec, buffer);
                        if (callstatus.HasError)
                        {
                            Console.WriteLine ("Error Reading Encoded Message: {0}", callstatus.ErrorMessage);
                            return;
                        }
                        Console.WriteLine ("Encoded message read from {0}", msgFileSpec);
                        string encoded = buffer.ToString ();

                        /* Decoding Message using ID2.iic */

                        buffer.Clear ();
                        if (!smex1.Decode (encoded, buffer))
                        {
                            Console.WriteLine ("Failed to Decode Message");
                            return;
                        }
                        else
                        {
                            Console.WriteLine ("Decoded Message: \n{0}", buffer.ToString ());
                        }
                    }
                }
                
                Console.WriteLine ("\nSample finished running");
            }
        }
    }
}