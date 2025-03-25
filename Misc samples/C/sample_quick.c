/*! \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_delegate.c

    \brief  Sertainty delgate management sample program using C language interface.  Uses XML to create ID.

    \author   Greg Smith
    \date     02/17/2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpquick_c.h"
#include "uxplicense_c.h"
#include "uxlbytearray_c.h"
#include "uxpsys_c.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(UXP_CLIENT)
#undef UXP_CLIENT
#endif

/**
 * Get challenge from the user
 * @param prompt Challenge prompt.
 * @param response User response.
 * @param prefix Response prefix for batch mode
 * @return True if accepted.  False if canceled
 */
static int getResponse(const uxlByteArray *prompt, uxlByteArray *response, const char *prefix);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpquickSandBox sandbox;
  int status;
  int done,i;
  uxlByteArray *buffer, *id;
  uxlByteArray *prompt, *response;
  char wrk[1000], wrk2[100];

  printf("\n\nSample using UXP Quick API\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();
  id = uxpba_newHandle();
  prompt = uxpba_newHandle();
  response = uxpba_newHandle();

  /* Set up log file and initialize library.  Must do this before any active calls. */

  uxpsys_initLibrary(buffer, argc, argv,
                     "sertainty.lic", "SertintyONE",
                     "SampleQuick", "Sample 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  /* Create a new UXP document with the ID */

  sandbox = uxpquick_newSandBox();

  /* Create a new ID */

  printf("Creating identity: SampleUser@myemail.com\n");

  strcpy(wrk, "username=SampleUser@myemail.com");

  for (i = 1; i <= 10; i++)
  {
    sprintf(wrk2, ":Challenge %d=MyResponse %d", i, i);
    strcat(wrk, wrk2);
  }

  uxpba_setData(buffer, wrk, strlen(wrk));

  if (!uxpquick_newIdentity(sandbox, buffer, id))
  {
    printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
    exit(1);
  }

  printf("\nCreating new UXP: sample.uxp\n");
  printf("Protecting document data.pdf\n");

  if (!uxpquick_openNewUxp(sandbox, id, "$(SAMPLE)/temp/sample.uxp", 1))
  {
    printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
    exit(1);
  }

  /* Create a new virtual file inside the Data*/

  if (!uxpquick_filetoUxp(sandbox, "$(SAMPLE)/SampleData/data.pdf", ModifierReplace))
  {
    printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
    exit(1);
  }

  printf("Closing new UXP\n");

  uxpquick_closeUxp(sandbox);

  printf("Opening new UXP\n\n");

  printf("Credentials necessary to access UXP:\n\n");
  printf("  Username = SampleUser@myemail.com\n");
  printf("  Challenge 1 = MyResponse 1\n");
  printf("  Challenge 2 = MyResponse 2\n");
  printf("  ... \n");
  printf("  Challenge 10 = MyResponse 10\n\n");

  if (!uxpquick_openUxp(sandbox, "$(SAMPLE)/temp/sample.uxp", ShareReadOnly))
  {
    printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
    exit(1);
  }

  done = 0;

  while (!done)
  {
    status = uxpquick_authenticateUxp(sandbox);

    switch (status)
    {
      case 0:
        printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
        exit(1);

      case StatusAuthorized:
        printf("You're authorized\n");
        done = 1;
        break;

      case StatusNotAuthorized:
        printf("You're not authorized\n");
        return 1;

      case StatusChallenged:
        for (size_t i = 0; i < uxpquick_getChallengeCount(sandbox); i++)
        {
          uxpquick_getChallenge(sandbox, i, prompt);

          getResponse(prompt, response, "MyResponse");

          uxpquick_addResponse(sandbox, prompt, response);
        }
        break;

      default:
        break;
    }
  }

  /* Read the virtual file and write it back out to disk */

  printf("\nExtracting data.pdf to copy.pdf\n");

  if (!uxpquick_uxpToFile(sandbox, "data.pdf", "$(SAMPLE)/temp/copy.pdf", ModifierReplace))
  {
    printf("UXP error: %s\n", uxpquick_readError(sandbox, buffer));
    exit(1);
  }

  uxpquick_closeUxp(sandbox);

  /* Clean up */

  uxpba_freeHandle(id);
  uxpba_freeHandle(buffer);
  uxpba_freeHandle(prompt);
  uxpba_freeHandle(response);

  printf("\nSample finished running\n");

  return 0;
}

/**
 * Get challenge from the user
 * @param prompt Challenge prompt.
 * @param response User response.
 * @param prefix Response prefix for batch mode
 * @return True if accepted.  False if canceled
 */
static int getResponse(const uxlByteArray *prompt, uxlByteArray *response, const char *prefix)
{
  char value[1000];

  const char *ptr2 = uxpba_getData(prompt);
  char tmp[10];

  if (!strcmp(ptr2, "Username"))
  {
    strcpy(value,"SampleUser@myemail.com");
  }
  else
  {
    strncpy(tmp, ptr2 + 10, 2);
    sprintf(value, "%s %s", prefix, tmp);
  }

  uxpba_setData(response, value, strlen(value));

  return 1;
}
