#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ddeml.h>
#include "dde.h"

//#define _ddedebug

//Instance of DDE transaction, global									 
DWORD Inst = 0;
HCONV Conversation;
int Timeout = 5000;

//Initialize DDE
int initDDE(int timeout)
{
    //Improve this, check if Inst is 0 else uninitialize first
    if (DdeInitialize(&Inst, (PFNCALLBACK)DdeCallback, APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0) == DMLERR_NO_ERROR)
    {
        #ifdef _ddedebug
        printf("Initialized DDE\n");
        #endif
        Timeout = timeout;
        return 0;
    }
    else
    {
        printf("Failed to initiliaze DDE\n");
        return 1;
    }
}

//Unitialize DDE
int uninitDDE()
{
    #ifdef _ddedebug
    printf("Uninitialized DDE\n");
    #endif
    DdeUninitialize(Inst);
    Inst = 0;
    Timeout = 5000;
    return 0;
}

//Connect to DDE Server
int connectDDE(char* ServiceName, char* TopicName)
{

    //DDE requires special handles to Strings
    HSZ ServiceHSZ = DdeCreateStringHandle(Inst, ServiceName, 0);
    HSZ TopicHSZ = DdeCreateStringHandle(Inst, TopicName, 0);

    //Setup the connection between DDE client and server
    Conversation = DdeConnect(Inst, ServiceHSZ, TopicHSZ, NULL);

    DdeFreeStringHandle(Inst, ServiceHSZ);
    DdeFreeStringHandle(Inst, TopicHSZ);

    //If we recieve a handle then connection atempt was sucessul
    if (Conversation == NULL)
    {
        printf("Failed to Connect to DDE Server. Instance %d \n", Inst);
        DDEError(DdeGetLastError(Inst));
        return 1;
    }
    else
    {
        #ifdef _ddedebug
        printf("Connected DDE\n");
        #endif
        return 0;
    }
}

//Complete a DDE query
char* queryDDE(char* ItemName, DWORD* DataLength)
{
    char* DataStr = NULL;

    HSZ ItemHSZ = DdeCreateStringHandle(Inst, ItemName, CP_WINANSI);

    //Get data in DDE format from the server
    HDDEDATA DataHandle = DdeClientTransaction(NULL, 0, Conversation, ItemHSZ, CF_TEXT, XTYP_REQUEST, Timeout, NULL); //need to check about NULL vs. nil here

    DdeFreeStringHandle(Inst, ItemHSZ);
       
    //If we get a handle to it we contrinue
    if (DataHandle == NULL)
    {
        printf("DDE Query \"%s\" Failed\n", ItemName);
        DDEError(DdeGetLastError(Inst));
    }        
    else
    {
        #ifdef _ddedebug
        printf("Completed DDE Transaction\n");
        #endif

        //Find out how much data we've got
        DdeAccessData(DataHandle, DataLength);
        DataStr = malloc(*DataLength);
                        
        //Convert the data to char
        DdeGetData(DataHandle, DataStr, *DataLength, 0);
            
        #ifdef _ddedebug            
        printf("Converted DDE Data %d byte(s)\n", *DataLength);
        printf("%s\n",DataStr);
        #endif

        DdeUnaccessData(DataHandle);
    }

    return DataStr;
}

//Disconnect from DDE server
int disconnectDDE()
{
    #ifdef _ddedebug
    printf("Disconnected DDE\n");
    #endif
    DdeDisconnect(Conversation);
}

/*
 * If you are making lots of queries to the same server, may be necessary to
 * Establist one connection first, the complete the transactions
 */
char* doDDE(char* ServiceName, char* TopicName, char* ItemName, DWORD* DataLength, int timeout)
{
    char* DataStr = NULL;
   
    if (initDDE(timeout) == 0)
    {
        if (connectDDE(ServiceName, TopicName) == 0)
        {
            DataStr = queryDDE(ItemName, DataLength);
            disconnectDDE();
        }
        uninitDDE();
    }
  
    return DataStr;
}

//Just report the error type back to the console
void DDEError(DWORD Error)
{
	switch (Error)
	{
	case DMLERR_ADVACKTIMEOUT:
      printf("DMLERR_ADVACKTIMEOUT\n");
      break;
	case DMLERR_BUSY:
      printf("DMLERR_BUSY\n");
      break;
	case DMLERR_DATAACKTIMEOUT:
	  printf("DMLERR_DATAACKTIMEOUT\n");
      break;
	case DMLERR_DLL_NOT_INITIALIZED:
	  printf("DMLERR_DLL_NOT_INITIALIZED\n");
      break;
    case DMLERR_DLL_USAGE:
	  printf("DMLERR_DLL_USAGE\n");
      break;
	case DMLERR_EXECACKTIMEOUT:
	  printf("DMLERR_EXECACKTIMEOUT\n");
      break;
	case DMLERR_INVALIDPARAMETER:
	  printf("DMLERR_INVALIDPARAMETER\n");
      break;
	case DMLERR_LOW_MEMORY:
	  printf("DMLERR_LOW_MEMORY\n");
      break;
	case DMLERR_MEMORY_ERROR:
	  printf("DMLERR_MEMORY_ERROR\n");
      break;
	case DMLERR_NO_CONV_ESTABLISHED:
	  printf("DMLERR_NO_CONV_ESTABLISHED\n");
      break;
	case DMLERR_NOTPROCESSED:
	  printf("DMLERR_NOTPROCESSED\n");
      break;
	case DMLERR_POKEACKTIMEOUT:
	  printf("DMLERR_POKEACKTIMEOUT\n");
      break;
	case DMLERR_POSTMSG_FAILED:
	  printf("DMLERR_POSTMSG_FAILED\n");
      break;
	case DMLERR_REENTRANCY:
	  printf("DMLERR_REENTRANCY\n");
      break;
	case DMLERR_SERVER_DIED:
	  printf("DMLERR_SERVER_DIED\n");
      break;
	case DMLERR_SYS_ERROR:
	  printf("DMLERR_SYS_ERROR\n");
      break;
	case DMLERR_UNADVACKTIMEOUT:
	  printf("DMLERR_UNADVACKTIMEOUT\n");
      break;
	case DMLERR_UNFOUND_QUEUE_ID:
	  printf("DMLERR_UNFOUND_QUEUE_ID\n");
      break;
    }
}

//Call back function for DDE, not really needed in this example
HDDEDATA CALLBACK _export DdeCallback (UINT iType, UINT iFmt, HCONV hCconv, HSZ hsz1,  HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2)
{
    unsigned char szBuffer[128];

	switch (iType)
	{
	case XTYP_DISCONNECT:
		return (HDDEDATA) TRUE; 
		break;
	case XTYP_ADVDATA:
        return (HDDEDATA) DDE_FACK;
		break;
    }
    return NULL;
}
