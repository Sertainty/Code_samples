/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_workflow_sm.c

    \brief    Sertainty SmartMessage sample program using C language interface.
              New ID is used as a single sign-on object. SmartMessage created linking ID.

                (1) Create ID from XML
                (2) New ID is used to in a single sign-on session
                (3) A SmartMessage is creating linking the ID
                (4) Closes the UXP
                (5) Authenticates success / fail
                (5) on success, extracts the data
                (7) closes UXP
 
    \details    A UXP SmartMessage requires authentication of the prospective user.  Access will only be
                granted after a successful authentication.  The process of authentication can
                be tedious if a user accesses multiple UXP objects or the same UXP object
                multiple times.  As a convenience, a Sertainty ID can be used as a single
                sign-on session, which will automatically authenticate UXP objects / SmartMessages that
                were created using the ID.

                For authentication, there are two approaches to programmatically seeking
                authorization.  The first approach is to declare a function callback that
                is called when the system presents challenges.  The callback function is
                given the list of challenges that it must process and return.  The callback
                function is called until resolution is reached.

                The second uses a looping process to allow the program to handle the
                challenge list manually.  Responses are then given back to the system and
                the authentication loop continues until a resolution is reached.

    \author   Greg Smith
    \date     10/30/2019
 
    \edited   Melani Smith Weed
    \date     27-jul-2020

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxplist_c.h"
#include "uxlbytearray_c.h"
#include "uxpcredential_c.h"
#include "uxpmsgid_c.h"
#include "uxpsys_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxpChallengeHandle ch);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
  uxpIdHandle idHandle;
  uxlByteArray *buffer, *buffer2;
  uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
  const char *idXmlSpec = "$(SAMPLE)/SampleData/sampleid.xml";
  const char *idFileSpec = "$(SAMPLE)/temp/sampleid.iic";
  int status = StatusChallenged;
  int cnt, i;
  static const char *test_message = "This is a test buffer";

  /* Can substitute sampleid.xml with other sample identity xml examples along with the associated *.iic created from the xml */
    
  printf("\nSample SmartMessage using C\n\n");

  /* Allocate a new buffer.  Automatically zeros it. */

  buffer = uxpba_newHandle();
  buffer2 = uxpba_newHandle();

  /* Set up log file and initialize library.  Must do this before any active calls. */

  uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                     "SampleSmC", "Sample SmartMessage 2.0.0");
  if (uxpba_getSize(buffer))
  {
    printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    return 1;
  }

  uxpMsgIdHandle msg;

  /* Create a new ID from the sampleid.xml ID definition */

  uxpsys_fileReadAll(callstatus, idXmlSpec, buffer);
  if (uxpsys_hasError(callstatus))
  {
    const char *errMsg = uxpsys_getErrorMessage(callstatus);
    printf("Error reading ID: %s\n", errMsg);
    return 1;
  }

  uxpid_publishToFile(callstatus,
                      idFileSpec,
                      uxpba_getData(buffer),
                      ModifierReplace);
  if (uxpsys_hasError(callstatus))
  {
    const char *errMsg = uxpsys_getErrorMessage(callstatus);
    printf("Error creating ID: %s\n", errMsg);
    return 1;
  }

  printf("Opening ID for SmartMessage session\n\n");

  printf("Credentials necessary to initiate SSO session:\n");
  printf("  Username = SampleUser@myemail.com\n");
  printf("  Challenge 1 = Response 1\n");
  printf("  Challenge 2 = Response 2\n");
  printf("  ... \n");
  printf("  Challenge 10 = Response 10\n\n");

  idHandle = uxpid_newHandle();

  uxpid_openSessionFromFile(idHandle, idFileSpec, 0, 0);
  if (uxpsys_hasError(uxpid_errorStatus(idHandle)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpid_errorStatus(idHandle));
    printf("Error opening UXP: %s\n", errMsg);
    return 1;
  }

  while (status != StatusAuthorized)
  {
    status = uxpid_authenticate(idHandle);

    switch (status)
    {
      case StatusAuthorized:
        printf("\nSSO session authorized\n");
        break;

      case StatusChallenged:
        cnt = uxpid_getChallengeCount(idHandle);

        for (i = 0; i < cnt; i++)
        {
          uxpChallengeHandle ch = uxpid_getChallenge(idHandle, i);

          if (!getResponse(ch))
          {
            printf("\nSSO session not authorized\n");
            exit(1);
          }

          uxpid_addResponse(idHandle, ch);

          uxpch_freeHandle(ch);
        }
        break;

      case StatusCanceled:
        printf("SSO session authorization canceled\n");
        exit(1);

      default:
        printf("\nSSO session not authorized\n");
        exit(1);
    }
  }

  /* Now that we have a session, we can create a SmartMessage
   *
   * To create a SmartMessage, the creator must be authenticated
   * via a valid ID session.  Once authenticated, the user
   * can now create a SmartMessage that can either be open
   * by the current ID or by a designated recipient ID.  If
   * the message is to be opened by this ID, then a second
   * exchange ID is null.  If the message is to be opened by
   * the second ID, then one must establish an exchange session
   * using the recipient ID.  The SmartMessage is then created
   * using the recipient ID and can only be opened and read by
   * that authenticated user.
   *
   * For this example, the creator is also the reader, so an
   * exchange session is not use.
   */

  printf("Opening new SmartMessage\n\n");

  msg = uxpmsgid_newHandle();

  uxpmsgid_setLinkedID(msg, idHandle);

  /* Create SmartMessage */

  printf("Encoding 'This is a test buffer' with minimal size flag set\n\n");

  uxpMsgIdFileHandle sm = uxpmsgid_openNewBuffer(msg, buffer, 0, -1, 0, ModifierMinSize);
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error opening new buffer: %s\n", errMsg);
    return 1;
  }

  uxpmsgid_append(msg, sm, test_message, (int)strlen(test_message));
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error writing message: %s\n", errMsg);
    return 1;
  }

  uxpmsgid_close(msg, sm);

  /* Read SmartMessage */

  sm = uxpmsgid_openBuffer(msg, uxpba_getData(buffer), (int)uxpba_getSize(buffer));
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error opening message: %s\n", errMsg);
    return 1;
  }

  /* Get the message properties */

  printf("SmartMessage properties\n\n");

  printf("Message size: %lld\n", uxpba_getSize(buffer));

  uxpmsgid_getProperties(msg, sm, buffer2);

  printf("%s\n", uxpba_getData(buffer2));

  uxpmsgid_read(msg, sm, buffer2, -1);
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error opening message: %s\n", errMsg);
    return 1;
  }

  uxpmsgid_close(msg, sm);

  printf("Decoded message: %s\n", uxpba_getData(buffer2));

  printf("Encoding 'This is a test buffer' with complete property list\n\n");

  sm = uxpmsgid_openNewBuffer(msg, buffer, 0, -1, 0, 0);
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error opening new buffer: %s\n", errMsg);
    return 1;
  }

  uxpmsgid_append(msg, sm, test_message, (int)strlen(test_message));
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error writing message: %s\n", errMsg);
    return 1;
  }

  uxpmsgid_close(msg, sm);

  /* Read SmartMessage */

  sm = uxpmsgid_openBuffer(msg, uxpba_getData(buffer), (int)uxpba_getSize(buffer));
  if (uxpsys_hasError(uxpmsgid_errorStatus(msg)))
  {
    const char *errMsg = uxpsys_getErrorMessage(uxpmsgid_errorStatus(msg));
    printf("Error opening message: %s\n", errMsg);
    return 1;
  }

  /* Get the message properties */

  printf("SmartMessage properties\n\n");

  printf("Message size: %lld\n", uxpba_getSize(buffer));

  uxpmsgid_getProperties(msg, sm, buffer2);

  printf("%s\n", uxpba_getData(buffer2));

  uxpmsgid_read(msg, sm, buffer2, -1);
  /*
    if (uxpsys_hasError(msg))
    {
      const char *errMsg = uxpsys_getErrorMessage(idHandle);
      printf("Error opening message: %s\n", errMsg);
      return 1;
    }
  */

  uxpmsgid_close(msg, sm);

  printf("Decoded message: %s\n", uxpba_getData(buffer2));

  /* Close the SmartMessage session */

  printf("\nSample SmartMessage session closing\n");

  uxpid_closeSession(idHandle);

  /* Free library and work buffer */

  uxpba_freeHandle(buffer);
  uxpba_freeHandle(buffer2);
  uxpid_freeHandle(idHandle);
  uxpsys_freeCallStatusHandle(callstatus);

  printf("\nSample finished running\n");

  return 0;
}

/**
 * Get challenge from the user
 * @param ch Challenge handle
 * @return True if accepted.  False if canceled
 */
static int getResponse(uxpChallengeHandle ch)
{
  char value[1000];
  uxlByteArray *prompt = uxpba_newHandle();

  uxpch_getPrompt(ch, prompt);
  uxpch_startTimer(ch);

  if (!strcmp(uxpba_getData(prompt), "Username"))
  {
    uxpch_setValueString(ch, "SampleUser@myemail.com");
  }
  else
  {
    strcpy(value, "Response ");

    strcat(value, uxpba_getData(prompt) + 10);
    uxpch_setValueString(ch, value);
  }

  uxpch_endTimer(ch);
  uxpba_freeHandle(prompt);

  return 1;
}
