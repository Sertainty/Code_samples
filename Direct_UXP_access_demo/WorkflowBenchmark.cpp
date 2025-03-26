#include "uxlcommon.h"
#include "uxpfile.h"
#include "uxpfs.h"
#include "uxlfileinfo.h"
#include "uxpsys.h"

#include <iostream>
#include <fstream>

/* Workflow Benchmark */

int main(int argc, char *argv[])
{
  int start,end, target, tmptarget;
  uxl::bytearray buf;
  std::string tmp;
  uxl::variant recbuf,mult_str;
  int maxsize, maxfetch, tmpmult, handle;
  int opt, newcache, silent, i, mult, fsize = 888824;
  std::string vname = "$(SAMPLE)/8-WorkflowDemo/Workflow.data";
  printf("\nWorkflow Test\n\n");
  int sample, cachesize, pagesize;
  std::map<std::string, uxl::variant> v;
  std::map<std::string, uxl::variant> attributes;
  uxp::file sf;

  uxp::sys::processStandardMacros(vname);

  try
  {
    uxp::sys::initializeLibrary(argc, argv,"sertainty.lic","SertintyONE");
  }
  catch (uxl::exception &e)
  {
    std::cout << e.what() << std::endl;
    exit(1);
  }

  printf("The multiplier directs the engine to replicate the same input\n");
  printf("data 'N' times, thus, producing a desired sample size.  The current\n");
  printf("source file contains 501 records, so multiplying by 2 would\n");
  printf("create 1002 records.\n");

  for (;;)
  {
    printf("\nSetting up new UXP\n\n");
    mult_str = uxp::sys::promptUser("Data multiplier <Exit>? ", "EXIT");
    if (uxl::string::toUpper(mult_str.toString()) == "EXIT")
      break;

    mult = mult_str.toInt();
    mult_str = uxp::sys::promptUser("Default page size <1000>? ", "1000");
    pagesize = mult_str.toInt();
    mult_str = uxp::sys::promptUser("Default cache size <0>? ", "0");
    cachesize = mult_str.toInt();
    mult_str = uxp::sys::promptUser("Compressed <False>? ", "False");
    if (uxl::string::toUpper(mult_str.toString()) == "FALSE")
      opt = 0;
    else
      opt = ModifierCompress;

    printf("\nCreating UXP\n");

    try
    {
      sf.openNewFile("$(SAMPLE)/8-WorkflowDemo/Workflow.uxp", "$(SAMPLE)/8-WorkflowDemo/Workflow.kcl", KclFile, ModifierReplace, 0);

      uxp::openVFHandle *handle;
      std::ifstream infile;
      uxl::fileinfo info(vname);
      char *tmpbuf;
      uxp::virtualFile fs;

      if (pagesize <= 0)
        pagesize = 8192;

      tmpbuf = (char *)malloc(pagesize + 1);

      try
      {
        handle = sf.newVirtualFile("data", pagesize, cachesize, opt);

        std::cout << "  To be processed: " << mult << std::endl;

        while (mult--)
        {
          infile.open(vname, std::ifstream::binary);

          for (;;)
          {
            infile.read(tmpbuf, pagesize);

            if (infile.gcount())
              sf.writeVirtualFile(handle, uxl::bytearray(tmpbuf, infile.gcount()));

            if (infile.eof())
              break;
          }

          infile.close();

          if (mult && ((mult % 10) == 0))
            std::cout << "  To be processed: " << mult << std::endl;
        }

        sf.closeVirtualFile(handle);

        sf.getVirtualFileInfo(fs, "data");
        maxsize = fs.getExtSize();
        std::cout << "Virtual file size: " << maxsize << " bytes" << std::endl;

        free(tmpbuf);
      }
      catch (uxl::exception &e)
      {
        free(tmpbuf);
        maxsize = 0;
      }

      attributes["NAME"] = "Workflow Test";
      attributes["DESCRIPTION"] = "Tests a workflow UXP for data access";

      sf.setAttributes(attributes);

      printf("\n");

      sf.getVirtualFileInfoXML("data", buf);
      printf("%s\n", buf.data());

      handle = sf.openVirtualFile("data", ModeRead);

      for (;;)
      {
        silent = false;

        printf("\n");
        recbuf = uxp::sys::promptUser("Number of reads <Back>? ", "Back");
        if (uxl::string::toUpper(recbuf.toString()) == "BACK")
          break;

        sample = recbuf.toInt();

        recbuf = uxp::sys::promptUser("Maximum buffer size <500>? ", "500");
        maxfetch = recbuf.toInt();
        if (maxfetch <= 0)
          continue;
        
        newcache = cachesize;

        tmp = uxl::string::format("Cache size <%1>? ", cachesize);

        recbuf = uxp::sys::promptUser(tmp, cachesize);
        newcache = recbuf.toInt();
        if (newcache == -1)
          newcache = cachesize;
        
        sf.setVirtualFileCacheSize(handle, newcache);

        recbuf = uxp::sys::promptUser("Show progress <True>? ", "true");
        silent = (uxl::string::toUpper(recbuf.toString()) == "FALSE");
        
        newcache = cachesize;

        /* Random sampling */

        uxp::sys::startStats("Test started");

        for (i = 1; i <= sample; i++)
        {
          start = uxp::sys::getRandomNumber(1, maxsize - maxfetch);

          if (!silent)
            std::cout << uxl::string::format("Read: %1  Offset: %2   %3\n", i, start, start + maxfetch);

          sf.seekVirtualFile(handle, start);
          sf.readVirtualFile(handle, buf, maxfetch);
        }
        
        uxp::sys::endStats("Test ended");
      }
      
      sf.closeVirtualFile(handle);

      /* Try an export */

      tmp = uxl::string::format("\nTesting export ... %1 bytes", maxsize);

      uxp::sys::startStats(tmp);

      sf.exportVirtualFile("data", "$(SAMPLE)/temp/tmp.export", ModifierReplace);

      uxp::sys::endStats("Export completed");

      sf.close();

      printf("\nClosing UXP\n");
    }
    catch (uxl::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
  }
}
