#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ddeml.h>

int initDDE(int timeout);
int connectDDE(char* ServiceName, char* TopicName);
char* queryDDE(char* ItemName, DWORD* DataLength);
int disconnectDDE();
int uninitDDE();
void DDEError(DWORD Error);
char* doDDE(char* ServiceName, char* TopicName, char* ItemName, DWORD* DataLength, int timeout);
HDDEDATA FAR PASCAL _export DdeCallback (UINT iType, UINT iFmt, HCONV hCconv, HSZ hsz1,  HSZ hsz2, HDDEDATA hData, DWORD dwData1, DWORD dwData2);

