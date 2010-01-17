/*
 Copyright (c) 2008 by contributors:

 * Damitha Premadasa
 * Nilendra Weerasinghe
 * Thilina Dampahala
 * Waruna Ranasinghe - (http://warunapw.blogspot.com)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
/*
// //
// // PROJECT  : HPC4Finance
// // MODULE   : HPC4FServer which runs in Master Node
// // FILE     : HPC4FServer.cpp
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : The implementation of the HPC4FServer class is done here. Each 
// 		 request is processed using a new thread.
// // TODO     : Failover and Load Balancing
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 13-Nov-2008 : Implemented 'WriteErrorLog()', 'WriteAccessLog()'
//               Modified: 14-Jan-2009 : Handle tickets -> get a ticket request and return a unique ticket id.
//               Modified: 19-Jan-2009 : Initialize pz_ErrorMSG and pmap_Tickets, free pmap_Tickets in distructor
//               		       : Bug fix: chek whether the original ticket holder made the request
//               		       : Modified WriteAccessLog to print a customized message
// //
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "HPC4FServer.h"
#include "RequestHandler.h"

//**************************************************************************************************
HPC4FServer::HPC4FServer()
{	
	i_Port = 8045;
	pz_HostAddr = ANY_ADDR;
	f_ErrorLog = stdout;
	f_AccessLog = stdout;
	printf("Port: %d\n", i_Port);
	bzero(z_ClientAddr, IP_ADDR_LEN);
	WriteAccessLog();
	pmap_Tickets = new map<int, char*>();
	tpool = new Tpool(HPC4FSERVER_MIN_THREADS, HPC4FSERVER_MAX_QUEUE, 0);
	pz_ErrorMSG = NULL;	
}

//**************************************************************************************************
HPC4FServer::HPC4FServer( char *pzHost, int iPort )
{
	i_Port = iPort;
	if (pzHost == ANY_ADDR)
		pz_HostAddr = ANY_ADDR;
	else
		pz_HostAddr = strdup(pzHost);
	f_ErrorLog = stdout;
	f_AccessLog = stdout;
	bzero(z_ClientAddr, IP_ADDR_LEN);
	WriteAccessLog();
	pmap_Tickets = new map<int, char*>();
	tpool = new Tpool(HPC4FSERVER_MIN_THREADS, HPC4FSERVER_MAX_QUEUE, 0);
//	printf("Port: %d \tHost: %s\n", this->i_Port, this->pz_HostAddr);
	pz_ErrorMSG = NULL;	
}

//**************************************************************************************************
HPC4FServer::HPC4FServer( char *pzHost, int iPort, FILE *fAccessLog, FILE *fErrorLog)
{
	i_Port = iPort;
	if (pzHost == ANY_ADDR)
		pz_HostAddr = ANY_ADDR;
	else
		pz_HostAddr = strdup(pzHost);
	if (fAccessLog)
		f_AccessLog = fAccessLog;
	else
		f_AccessLog = stdout;
	if (fErrorLog)
		f_ErrorLog = fErrorLog;
	else
		f_ErrorLog = stdout;
	bzero(z_ClientAddr, IP_ADDR_LEN);
	WriteAccessLog();
	pmap_Tickets = new map<int, char*>();
	tpool = new Tpool(HPC4FSERVER_MIN_THREADS, HPC4FSERVER_MAX_QUEUE, 0);
//	printf("Port: %d \tHost: %s\n", this->i_Port, this->pz_HostAddr);
	pz_ErrorMSG = NULL;	
}

//**************************************************************************************************
HPC4FServer::~HPC4FServer()
{
	if (pz_HostAddr)
		delete pz_HostAddr;
	tpool->Destroy(1);
	delete tpool;
	map<int, char*>::iterator it;
	for (it = pmap_Tickets->begin(); it != pmap_Tickets->end(); it++)
	{
		delete [](*it).second;
	}
	delete pmap_Tickets;
	if(pz_ErrorMSG != NULL)
		delete pz_ErrorMSG;
}

//**************************************************************************************************
#ifdef _WIN32
bool HPC4FServer::WinSockDLLVersionOK()
{
	WORD 	wVersionRequested;
	WSADATA wsaData;
	int 	iErr;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	iErr = WSAStartup( wVersionRequested, &wsaData );
	
	/* Found a usable winsock dll? */
	if( iErr != 0 ) 
		return false;
	
	/* Confirm that the WinSock DLL supports 2.2.
	* Note that if the DLL supports versions greater 
	* than 2.2 in addition to 2.2, it will still return
	* 2.2 in wVersion since that is the version we
	* requested
	* */
	
	if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
	
	/* Tell the user that we could not find a usable WinSock DLL.*/
		WSACleanup( );
		return false;
	}
	
	/* The WinSock DLL is acceptable. Proceed. */
	return true;
}
#endif

//**************************************************************************************************
bool HPC4FServer::CreateHPC4FServer()
{
	struct sockaddr_in ssAddr;

#ifdef _WIN32
	if(!WinSockDLLVersionOK())
		return false;
#endif
	
	i_ServerSock = socket(AF_INET, SOCK_STREAM, 0);
	if (i_ServerSock < 0)
	{
		//calling WSAGetLastError to get the error details in windows
		WriteErrorLog("", strerror(errno));
		printf("Error: %s\n", strerror(errno));
		return false;
	}

#ifdef SO_REUSEADDR
	int opt = 1;
	setsockopt(i_ServerSock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,sizeof(int));
#endif

	bzero(&ssAddr, sizeof(ssAddr));
	ssAddr.sin_family = AF_INET;

	if (pz_HostAddr == NULL)
	{
		ssAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		ssAddr.sin_addr.s_addr = inet_addr(pz_HostAddr);
	}

	ssAddr.sin_port = htons((u_short)i_Port);

	if (bind(i_ServerSock,(struct sockaddr *)&ssAddr,sizeof(ssAddr)) <0)
	{
		WriteErrorLog("", strerror(errno));
		close(i_ServerSock);
		return false;
	}

	if(listen(i_ServerSock, MAX_QUEUE_LEN) < 0)
	{
		WriteErrorLog("", strerror(errno));
		close(i_ServerSock);
		return false;
	}
	
	return true;
}

//**************************************************************************************************
bool HPC4FServer::SetErrorLog( FILE *fp )
{
	f_ErrorLog = fp;
	return true;
}

//**************************************************************************************************
bool HPC4FServer::SetAccessLog( FILE *fp )
{
	f_AccessLog = fp;
	return true;
}

//**************************************************************************************************
enum eSelectStatus HPC4FServer::GetConnection( struct timeval *stTimeout )
{
	int iResult;
	int iClientSock;
	int issAddrLen;
	char *pzIP;
	struct sockaddr_in ssAddr;
	fd_set fdSet;
	pthread_t thread;

	FD_ZERO(&fdSet);
	FD_SET((unsigned)i_ServerSock, &fdSet);
	iResult = 0;

	while(iResult == 0)
	{
		iResult = select(i_ServerSock + 1, &fdSet, 0, 0, stTimeout);
		if (iResult < 0)
			return ERR;
		if (stTimeout != 0 && iResult == 0)
			return TIMEOUT;
		if (iResult > 0)
			break;
	}

	bzero(&ssAddr, sizeof(ssAddr));
	issAddrLen = sizeof(ssAddr);

	iClientSock = accept(i_ServerSock,(struct sockaddr *)&ssAddr, (socklen_t*)&issAddrLen);
	pzIP = inet_ntoa(ssAddr.sin_addr);
	strcpy(z_ClientAddr, pzIP);
	
//	WriteAccessLog();	

	/* The request is handled by a seperate thread. The main thread will NOT wait
	 * untill the thread exit. It will continue to listen to new requests
	 * */	
	RequestHandler *pReqHan = new RequestHandler(iClientSock, pzIP, i_Port, i_ServerSock, f_ErrorLog, f_AccessLog);
	pReqHan->ReadRequest();
	if (pReqHan->IsTicket())
	{
		char pzReferenceID[33];
		int iTicket = tpool->GetNextReferenceID();
		sprintf(pzReferenceID, "%d", iTicket);
		if (pReqHan->WriteSockData(pzReferenceID, strlen(pzReferenceID)) > 0)
		{
			(*pmap_Tickets)[iTicket] = strdup(z_ClientAddr);
		}
		else
		{
			printf("Error: %s\n", strerror(errno));
			WriteErrorLog("", strerror(errno));
		}
		return TIMEOUT;
	}
	else if (!pReqHan->IsAbortRequest())
	{
		int iTicket = pReqHan->GetTicket();
		if ((*pmap_Tickets)[iTicket] != NULL)
		{
			if((strcmp((*pmap_Tickets)[iTicket], z_ClientAddr)) != 0)
			{
				pReqHan->WriteSockData("error,Access Denied!", strlen("error,Access Denied!"));
				pz_ErrorMSG = strdup("Access Violation");
				printf("Caution: %s\n", pz_ErrorMSG);
				WriteAccessLog();
				return TIMEOUT;
			}
			else
			{
				pmap_Tickets->erase(iTicket);
			}
		}
		else
		{
			pReqHan->WriteSockData("error,Access Denied!", strlen("error,Access Denied!"));
			pz_ErrorMSG = strdup("Access Violation");
			printf("Caution: %s\n", pz_ErrorMSG);
			WriteAccessLog();
			return TIMEOUT;
		}	
	}

	////////////////////////////////////////////////////////////// Check the thread function- it shud b removed
	if (tpool->AddWork((void*)0, (void*)pReqHan) == -1)
	{
		WriteErrorLog("", "TPOOL ERROR\n"); // CHK whether errno set when there is a thread error
                return ERR;//EXIT_FAILURE;
	}

	return TIMEOUT;//PENDING_REQUEST;
}

void HPC4FServer::WriteErrorLog( char *pzLevel, char *pzMSG )
{
	char	dateBuf[30];
	struct 	tm *timePtr;
	time_t	clock;


	if (f_ErrorLog == NULL)
		return;
	clock = time(NULL);
	timePtr = localtime(&clock);
	strftime(dateBuf, 30, "%a %b %d %T %Y",  timePtr);
	fprintf(f_ErrorLog, "%s:%s [%s] %s\n", __FILE__, __LINE__, dateBuf, pzMSG);
	fflush(f_ErrorLog);
}

//**************************************************************************************************
void HPC4FServer::WriteAccessLog()
{
	char	dateBuf[30];
	struct 	tm *timePtr;
	time_t	clock;
	int	responseCode;


	if (f_AccessLog == NULL)
		return;
	clock = time(NULL);
	timePtr = localtime(&clock);
	strftime(dateBuf, 30, "%d/%b/%Y:%T %Z",  timePtr);
	if (*z_ClientAddr != 0)
	{
		if (pz_ErrorMSG)
			fprintf(f_AccessLog, "%s - - [%s] -- %s \n", z_ClientAddr, dateBuf, pz_ErrorMSG);
		else
			fprintf(f_AccessLog, "%s - - [%s] \n", z_ClientAddr, dateBuf);
			
		fflush(f_AccessLog);
	}
	else
	{
		fprintf(f_AccessLog, "HPC4FServer is Started, Host: %s Port: %d - - [%s] \n", pz_HostAddr, i_Port, dateBuf);
		fflush(f_AccessLog);
	}
}

// **************************************************************************************************

