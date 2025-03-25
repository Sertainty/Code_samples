/*! 
    \copyright Sertainty Corporation, 2020.  All Rights Reserved.

    \file     sample_sm_exchange.c

    \brief    Sample program using C language interface and SMEX. This samples demonstrates how to create a Smart Message Exchange between two ID's.      
    \                 (1) Two on-demand, one-time temporary ID's are created, they will each be used to Encode and Decode messages (ID1.iic and ID2.iic)
    \                 (2) Two SMEX sessions are initialized
    \                 (3) Two tests are performed, one after the other:
    \                     - For the first test: ID1.iic will encode a smart message and ID2.iic will decode the smart message.
    \                     - For the second test: ID2.iic will encode a smart message and ID1.iic will decode the smart message.

    \author   Karim Lalani
    \date     06/20/2018
 
    \edited   Melani Smith Weed
    \date

    \edited   Melvin Valdez de la Roca
    \date     12/28/2021

    \note     Application expects to find the necessary source files in the current working folder.
 */

#include "uxpfile_c.h"
#include "uxpfile_common.h"
#include "uxplist_c.h"
#include "uxlbytearray_c.h"
#include "uxpvfile_c.h"
#include "uxpcredential_c.h"
#include "uxpid_c.h"
#include "uxpsys_c.h"
#include "uxpsmexchange_c.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int getLine(char **buf);

static char readchar(void);

/**
 * @brief Main entry point
 * @param argc Number of arguments
 * @param argv Array of argument tokens
 * @return 0 - success, 1 - error
 */
int main(int argc, char *argv[])
{
    long long size;
    int rc = 1;
    uxlByteArray *buffer;
    char g,o;
    int r = UXPSMEX_FAILURE;

    const char * msgFileSpec = "$(SAMPLE)/temp/msg.file";

    printf("\n\nSample Smid C Data Application\n\n");

    /* Allocate a new buffer.  Automatically zeros it. */

    buffer = uxpba_newHandle();

    /* Set up log file and initialize library.  Must do this before any active calls. */

    uxpsys_initLibrary(buffer, argc, argv, "sertainty.lic", "SertintyONE",
                        "SampleSmidC", "Sample 2.0.0");
    if (uxpba_getSize(buffer))
    {
        printf("Error initializing environment: %s\n", uxpba_getData(buffer));
    }
    else
    {
        uxpCallStatusHandle callstatus = uxpsys_newCallStatusHandle();
        uxpba_clearData(buffer);
        uxpSMExchangeHandle smex1 = uxpsmex_newHandle();
        uxpSMExchangeHandle smex2 = uxpsmex_newHandle();

        const char * srcId1Spec = "$(SAMPLE)/temp/ID1.iic";
        const char * srcId2Spec = "$(SAMPLE)/temp/ID2.iic";

        /* Initialize First SMEX Session */

        printf("Initializing SMEX1 Session ...\n");
        uxpba_clearData(buffer);
        r = uxpsmex_initSession(smex1,buffer);
        if(r == UXPSMEX_SUCCESS)
        {
            int mods = 0;
            mods = ModifierReplace;

            uxpsys_fileWriteAll(callstatus,srcId1Spec,uxpba_getData(buffer),uxpba_getSize(buffer),mods);
            if (uxpsys_hasError(callstatus))
            {
                const char *errMsg = uxpsys_getErrorMessage(callstatus);
                printf("Error writing ID: %s\n", errMsg);
                r = UXPSMEX_FAILURE;
            }
            else
            {
                printf("%s created successfully\n", srcId1Spec);
                size = uxpba_getSize(buffer); 
                printf("Buffer Size: %lld\n", size);
            }
        }
        else
        {
            printf("\nSMEX1 Initialization Failed");
        }

        /* Initialize Second SMEX Session */

        printf("\nInitializing SMEX2 Session ...\n");
        uxpba_clearData(buffer);
        r = uxpsmex_initSession(smex2,buffer);
        if(r == UXPSMEX_SUCCESS)
        {
            int mods = 0;
            mods = ModifierReplace;

            uxpsys_fileWriteAll(callstatus,srcId2Spec,uxpba_getData(buffer),uxpba_getSize(buffer),mods);
            if (uxpsys_hasError(callstatus))
            {
                const char *errMsg = uxpsys_getErrorMessage(callstatus);
                printf("\nError writing ID: %s\n", errMsg);
                r = UXPSMEX_FAILURE;
            }
            else
            {
                printf("%s created successfully\n", srcId2Spec);
                size = uxpba_getSize(buffer);
                printf("Buffer Size: %lld\n", size);
            }
        }
        else
        {
            printf("\nSMEX2 Initialization Failed");
        }

        if (r == UXPSMEX_FAILURE)
        {
            printf("\nError Initializing SMEX Token\n");
        }
        else
        {
            printf("\nSMEX Token initialized\n");
            const char * destId1Spec = "$(SAMPLE)/temp/ID1.iic";
            const char * destId2Spec = "$(SAMPLE)/temp/ID2.iic";

            /* TEST 1: Encode using 'ID1.iic', Decode using 'ID2.iic' */

            uxpba_clearData(buffer);
            uxpsys_fileReadAll(callstatus,destId2Spec,buffer);
            if (uxpsys_hasError(callstatus))
            {
                const char *errMsg = uxpsys_getErrorMessage(callstatus);
                printf("\nError Reading ID: %s\n", errMsg);
                r = UXPSMEX_FAILURE;
            }
            else
            {
                printf("\nSetting 'ID2' as destination file ...");
                size = uxpba_getSize(buffer);
                printf("\nBuffer Length: %lld", size);
                r = uxpsmex_startSession(smex1,uxpba_getData(buffer),uxpba_getSize(buffer));
                if (r == UXPSMEX_SUCCESS)
                {
                    printf("\nStartSession: True");
                }
                else
                {
                    printf("\nStartSession: False");
                }
            }

            if (r == UXPSMEX_FAILURE)
            {
                printf("\nError Starting SMEX1 Session\n");
            }
            else
            {
                printf("\nSMEX1 Session Started\n");
                uxpba_clearData(buffer);

                /* Encoding Message using ID1.iic */
                
                printf("\nEncoding Message using ID1.iic ...");
                char * msg = NULL;
                msg = "Hello ID2.iic!";
                size = strlen(msg);

                if (uxpsmex_encode(smex1,msg,size,buffer) == UXPSMEX_FAILURE)
                {
                    printf("\nFailed to Encode Message\n");
                    return 1;
                }
                else
                {
                    printf("\nMessage Encoded Successfully");
                    uxpsys_fileWriteAll(callstatus,msgFileSpec,uxpba_getData(buffer),uxpba_getSize(buffer),ModifierReplace);
                    if (uxpsys_hasError(callstatus))
                    {
                        const char *errMsg = uxpsys_getErrorMessage(callstatus);
                        printf("Error Writing Encoded Message: %s\n", errMsg);
                        return 1;
                    }
                    printf("\nEncoded message is written to %s\n",msgFileSpec);
                }

                printf("\nDecoding Message using ID2.iic ...");
                uxpba_clearData(buffer);
                uxpsys_fileReadAll(callstatus,msgFileSpec,buffer);
                if (uxpsys_hasError(callstatus))
                {
                    const char *errMsg = uxpsys_getErrorMessage(callstatus);
                    printf("Error Reading Encoded Message: %s\n", errMsg);
                    return 1;
                }
                printf("\nEncoded message read from %s\n",msgFileSpec);
                const char *msg_buf = uxpba_getData(buffer);
                size = uxpba_getSize(buffer);

                /* Decoding Message using ID2.iic */

                if (uxpsmex_decode(smex2,msg_buf,size,buffer) == UXPSMEX_FAILURE)
                {
                    printf("Failed to Decode Message\n");
                    return 1;
                }
                printf("Decoded message:\n%s\n",uxpba_getData(buffer));
            }

            /* TEST 2: Encode using 'ID2.iic', Decode using 'ID1.iic' */

            uxpba_clearData(buffer);
            uxpsys_fileReadAll(callstatus,destId1Spec,buffer);
            if (uxpsys_hasError(callstatus))
            {
                const char *errMsg = uxpsys_getErrorMessage(callstatus);
                printf("\nError Reading ID: %s\n", errMsg);
                r = UXPSMEX_FAILURE;
            }
            else
            {
                printf("\nSetting 'ID1' as destination file ...");
                size = uxpba_getSize(buffer); 
                printf("\nBuffer Length: %lld", size);
                r = uxpsmex_startSession(smex2,uxpba_getData(buffer),uxpba_getSize(buffer));
                if (r == UXPSMEX_SUCCESS)
                {
                    printf("\nStartSession: True");
                }
                else
                {
                    printf("\nStartSession: False");
                }
            }

            if (r == UXPSMEX_FAILURE)
            {
                printf("\nError Starting SMEX1 Session\n");
            }
            else
            {
                printf("\nSMEX2 Session Started\n");
                uxpba_clearData(buffer);

                /* Encoding Message using ID1.iic */

                printf("\nEncoding Message using ID2.iic ...");
                char * msg = NULL;
                msg = "Hi ID1.iic, I received your message!";
                size = strlen(msg);

                if (uxpsmex_encode(smex2,msg,size,buffer) == UXPSMEX_FAILURE)
                {
                    printf("\nFailed to Encode Message\n");
                    return 1;
                }
                else
                {
                    printf("\nMessage Encoded Successfully");
                    uxpsys_fileWriteAll(callstatus,msgFileSpec,uxpba_getData(buffer),uxpba_getSize(buffer),ModifierReplace);
                    if (uxpsys_hasError(callstatus))
                    {
                        const char *errMsg = uxpsys_getErrorMessage(callstatus);
                        printf("Error Writing Encoded Message: %s\n", errMsg);
                        return 1;
                    }
                    printf("\nEncoded message is written to %s\n",msgFileSpec);
                }

                printf("\nDecoding Message using ID1.iic ...");
                uxpba_clearData(buffer);
                uxpsys_fileReadAll(callstatus,msgFileSpec,buffer);
                if (uxpsys_hasError(callstatus))
                {
                    const char *errMsg = uxpsys_getErrorMessage(callstatus);
                    printf("Error Reading Encoded Message: %s\n", errMsg);
                    return 1;
                }
                printf("\nEncoded message read from %s\n",msgFileSpec);
                const char *msg_buf = uxpba_getData(buffer);
                size = uxpba_getSize(buffer);

                /* Decoding Message using ID1.iic */

                if (uxpsmex_decode(smex1,msg_buf,size,buffer) == UXPSMEX_FAILURE)
                {
                    printf("Failed to Decode Message\n");
                    return 1;
                }
                printf("Decoded message:\n%s\n",uxpba_getData(buffer));
            }
        }

        printf("\nSample finished running\n");
    }

    uxpba_clearData(buffer);
    uxpba_freeHandle(buffer);

    return rc;
}

static int getLine(char **buf)
{
    char c;

    int idx = 0;
    char *ptr = *buf;
    for (;;)
    {
        c = getchar();
        if (c == '\n' || c == '\r')
        break;
        idx++;
        *ptr++ = c;
    }

    *ptr = '\0';
    return idx;
}

static char readchar()
{
    int c = getchar();
    if(c == EOF || c == '\n') return c;
    int n;
    while((n = getchar()) != EOF && n != '\n');
    return (char)c;
}