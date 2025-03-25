using System;
using System.Collections.Generic;
using System.IO;
using System.Security.Cryptography;
using Sertainty;

namespace Webservice_UXP_Sample
{
  class Program
  {
    static void Main()
    {
      var url = "http://so-osx.example.com:5000/dataservices";
      var key = "12345678901234567890123456789012";

      string idName = "sampleid.iic";
      string uxpName1 = "sample.uxp";
      string uxpName2 = "sample2.uxp";
      string dataName1 = "data.pdf";
      string dataName2 = "data2.pdf";

      string sample = string.Format("{0}/Sertainty/developer/examples",
        Environment.GetFolderPath(Environment.SpecialFolder.UserProfile));
      string idFileSpec = string.Format("{0}/temp/{1}", sample, idName);
      
      string dataPdfSpec1 = string.Format("{0}/SampleData/{1}", sample, dataName1);
      string dataPdfSpec2 = string.Format("{0}/SampleData/{1}", sample, dataName2);
      string uxpSpec2 = string.Format("{0}/temp/{1}", sample, uxpName2);
      
      var ds = new WebService(url, key);
      Status status;
      var inlist = new Dictionary<string, string>
      {
        { "username", "SampleOne@myemail.com" },
        { "stop_time", "0" }
      };

      Console.WriteLine("Openning Service Session...");
      WebServiceResponse response;

      response = ds.CreateRequest("ds::openSession", inlist).GetResponse();
     
      if (!(status = response.Status).IsSuccess)
      {
        Console.WriteLine(status);
        return;
      }

      Console.WriteLine("Authenticating...");

      if(!AuthSessionOrUXP(ds))
      {
        Console.WriteLine("Unable to open session...");
        return;
      }

      Console.WriteLine("<First Test: Creating New UXP and Adding Files...");

      Console.WriteLine("Uploading ID File and Data Files to server...");

      var filelist = new Dictionary<string, string>
      {
        { idName, idFileSpec },
        { dataName1, dataPdfSpec1 },
        { dataName2, dataPdfSpec2 }
      };


      foreach (var file in filelist)
      {
        Console.WriteLine("Uploading {0} ...", file.Key);
        if (!UploadFile(ds, file.Key, file.Value))
        {
          Console.WriteLine("Upload failed for {0} ...", file.Key);
          return;
        }
      }

      Console.WriteLine("Creating new UXP on Server...");

      filelist.Remove(idName);
      if(!CreateNewUXPTest(ds,uxpName1,idName,filelist))
      {
        Console.WriteLine("Create UXP Test failed...");
      }
      Console.WriteLine("Opening UXP on Server...");

      if(!OpenUXPTest(ds,uxpName1,filelist))
      {
        Console.WriteLine("Open UXP Test failed...");
      }

      if(DownloadFile(ds,uxpName1,uxpSpec2))
      {
        Console.WriteLine("{0} downloaded to {1}",uxpName1,uxpSpec2);
      }
      else
      {
        Console.WriteLine("Failed to download {0}", uxpName1);
      }

      response = ds.CreateRequest("ds::closeSession").GetResponse();
      if(!(status = response.Status).IsSuccess)
      {
        Console.WriteLine(status);
      }
    }

    private static bool AuthSessionOrUXP(WebService ds, string uxpHandle = null)
    {
      var authPrefix = string.IsNullOrEmpty(uxpHandle) ? "ds" : "sf";

      var inlist = new Dictionary<string, string>();
      var authFunction = string.Format("{0}::authenticate", authPrefix);
      var getChFunction = string.Format("{0}::getChallenges", authPrefix);
      var setRespFunction = string.Format("{0}::setResponses", authPrefix);
      Status status;

      bool authorized = false;
      
      while (true)
      {
        inlist.Clear();
        if (!string.IsNullOrEmpty(uxpHandle))
        {
          inlist.Add("handle", uxpHandle);
        }
        var authRequest = ds.CreateRequest(authFunction, inlist);
        
        var authResponse = authRequest.GetResponse();
        
        if (!(status = authResponse.Status).IsSuccess)
        {
          Console.WriteLine(status);
          break;
        }
        var authStatus = int.Parse(authResponse.Data["authStatus"]);

        switch (authStatus)
        {
          case 16:
            Console.WriteLine("Authorized...");
            return true;
          case 512:
            Console.WriteLine("Challenged...");
            inlist.Clear();
            if (!string.IsNullOrEmpty(uxpHandle))
            {
              inlist.Add("handle", uxpHandle);
            }
            var getChReq = ds.CreateRequest(getChFunction, inlist);
            
            var getChRes = getChReq.GetResponse();
            if (!(status = getChRes.Status).IsSuccess)
            {
              Console.WriteLine(status);
              return false;
            }
            var challenges = getChRes.Data["challenges"];
            var responses = string.Empty;
            foreach (var challenge in challenges.Split("||"))
            {
              var chpair = challenge.Split("|");
              var chname = chpair[0];
              var chprompt = chpair[1];

              if (responses.Length > 0)
              {
                responses += "||";
              }

              Console.WriteLine("Challenge: {0}", chprompt);
              Console.Write("Response: ");
              var response = Console.ReadLine().Trim();
              responses += challenge + "|" + response;
            }
            inlist.Clear();
            if (!string.IsNullOrEmpty(uxpHandle))
            {
              inlist.Add("handle", uxpHandle);
            }
            inlist.Add("responses", responses);
            var setRespReq = ds.CreateRequest(setRespFunction, inlist);
            
            var setRespRes = setRespReq.GetResponse();
            if (!(status = setRespRes.Status).IsSuccess)
            {
              Console.WriteLine(status);
              return false;
            }
            break;
          default:
            Console.WriteLine("Not Authorized...");
            return false;
        }
      }
      return authorized;
    }

    private static bool UploadFile(WebService ds, string filename, string filepath)
    {
      Status status;
      var inlist = new Dictionary<string, string>
      {
        { "filename", filename },
        { "options", "Replace" }
      };

      var fileBytes = File.ReadAllBytes(filepath);
      inlist.Add("buffer", Convert.ToBase64String(fileBytes));
      var response = ds.CreateRequest("sf::uploadFile", inlist).GetResponse();
      if (!(status = response.Status).IsSuccess)
      {
        Console.WriteLine(status);
      }

      return status.IsSuccess;
    }

    private static bool DownloadFile(WebService ds, string filename, string filepath)
    {
      var inlist = new Dictionary<string, string>();
      
      Status status;
      inlist.Add("filename", filename);
      var response = ds.CreateRequest("sf::downloadFile", inlist).GetResponse();
      if (!(status = response.Status).IsSuccess)
      {
        Console.WriteLine(status);
        return false;
      }

      var buffer = response.Data["buffer"];
      var fileContents = Convert.FromBase64String(buffer);

      var file = File.Create(filepath);
      file.Write(fileContents, 0, fileContents.Length);

      file.Close();
      return true;
    }

    private static bool CreateNewUXPTest(WebService ds, string uxpname, string idname, IDictionary<string,string> filelist)
    {
      var inlist = new Dictionary<string, string>
      {
        { "filename", uxpname },
        { "id", idname },
        { "options", "IDFile|Replace" }
      };

      Status status;
      WebServiceResponse response;
      response = ds.CreateRequest("sf::newUxp", inlist).GetResponse();
      if((status = response.Status).IsSuccess)
      {
        Console.WriteLine("Adding files to UXP...");
        
        var filehandle = response.Data["handle"];
        
        foreach (var file in filelist)
        {
          inlist.Clear();
          inlist.Add("handle", filehandle);
          inlist.Add("name", file.Key);
          inlist.Add("filename", file.Key);
          inlist.Add("pagesize", "-1");
          inlist.Add("cachesize", "-1");
          inlist.Add("options", "Compress");

          response = ds.CreateRequest("sf::addFile", inlist).GetResponse();
          if (!(status = response.Status).IsSuccess)
          {
            break;
          }
        }

        Console.WriteLine("Verifying files in UXP...");
        VerifyFiles(ds, filehandle, filelist);

        Console.WriteLine("Closing UXP...");
        inlist.Clear();
        inlist.Add("handle", filehandle);

        status = ds.CreateRequest("sf::closeUxp", inlist).GetResponse().Status;
      }
      if (!status.IsSuccess)
      {
        Console.WriteLine(status);
      }
      return status.IsSuccess;
    }

    private static bool OpenUXPTest(WebService ds, string uxpname, IDictionary<string,string> filelist)
    {
      var inlist = new Dictionary<string, string>
      {
        { "filename", uxpname },
        { "mode", "ShareAll" }
      };

      Status status;

      var response = ds.CreateRequest("sf::openUxp", inlist).GetResponse();
      if (!(status = response.Status).IsSuccess)
      {
        Console.WriteLine(status);
      }

      var filehandle = response.Data["handle"];
      
      if(!AuthSessionOrUXP(ds,filehandle))
      {
        return false;
      }

      VerifyFiles(ds, filehandle, filelist);
      inlist.Clear();
      inlist.Add("handle", filehandle);
      status = ds.CreateRequest("sf::closeUxp", inlist).GetResponse().Status;

      return status.IsSuccess;
    }

    private static void VerifyFiles(WebService ds, string filehandle, IDictionary<string, string> filelist)
    {
      Status status;
      WebServiceResponse response;
      var inlist = new Dictionary<string, string>();
      foreach (var file in filelist)
      {
        inlist.Clear();
        inlist.Add("handle", filehandle);
        inlist.Add("virSpec", file.Key);

        response = ds.CreateRequest("sf::openFile", inlist).GetResponse();
        if ((status = response.Status).IsSuccess)
        {
          var vfhandle = response.Data["fileHandle"];

          inlist.Clear();
          inlist.Add("handle", filehandle);
          inlist.Add("fileHandle", vfhandle);
          inlist.Add("len", "-1");

          response = ds.CreateRequest("sf::readFile", inlist).GetResponse();
          if ((status = response.Status).IsSuccess)
          {
            var b64buffer = response.Data["buffer"];
            var remoteBuffer = Convert.FromBase64String(b64buffer);
            var localBuffer = new byte[remoteBuffer.Length];
            var localfile = File.OpenRead(file.Value);
            localfile.Read(localBuffer, 0, localBuffer.Length);
            localfile.Close();

            var b64Md5Remote = Convert.ToBase64String(MD5.HashData(remoteBuffer));
            var b64Md5Local = Convert.ToBase64String(MD5.HashData(localBuffer));

            Console.WriteLine("{0}: Remote file {1} Local", file.Key, b64Md5Remote == b64Md5Local ? "matches" : "doesn't match");
          }
          else
          {
            Console.WriteLine(status);
          }
          inlist.Clear();
          inlist.Add("handle", filehandle);
          inlist.Add("fileHandle", vfhandle);
          response = ds.CreateRequest("sf::closeFile", inlist).GetResponse();
          if (!(status = response.Status).IsSuccess)
          {
            Console.WriteLine(status);
            break;
          }
        }
      }
    }
  }
}
