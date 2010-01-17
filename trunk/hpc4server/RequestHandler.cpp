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
// // FILE     : RequestHandler.cpp
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : The implementation of the  HPC4FServer class is done here. Each
// 		 request is processed using a new thread.
// // TODO     : Failover
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 13-Nov-2008 : implemented 'WriteAccesslog()', 'WriteErrorLog()'
//               	   17-Nov-2008 : added 'StrTokenizer'
//               	   07-Jan-2009 : changed 'mpiexec' to 'mpdrun' to have an alias to the job
//              	   14-Jan-2009 : GetTicket, IsTicket, IsAbortRequest is added
//               	   15-Jan-2009 : GetMachineFileName aded for load balancing
// //
*/

#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include "RequestHandler.h"
#include "defs.h"

using namespace std;

//**************************************************************************************************
RequestHandler::RequestHandler(int iClientSock, char *pzClientIPAddr, int iPort, int iServerSock, FILE *fErrorLog, FILE *fAccessLog)
{
	i_ClientSock = iClientSock;
	i_Port = iPort;
	i_ServerSock = iServerSock;
	i_ReadBufRemain = 0;
	b_FullyReadSockData = false;
	pz_ReadBuf = NULL;
	pz_Request = NULL;
	bzero(z_ReadBuf, MAX_READ_BUF_LEN + 1);
	if (pzClientIPAddr)
		strncpy(z_ClientAddr, pzClientIPAddr, IP_ADDR_LEN);
	else
		*z_ClientAddr = 0;
	if (fErrorLog)
	{
		f_ErrorLog = fErrorLog;
	}
	else
		f_ErrorLog = NULL;
	if (fAccessLog)
	{
		f_AccessLog = fAccessLog;
	}
	else
		f_AccessLog = NULL;
}

//**************************************************************************************************
RequestHandler::~RequestHandler()
{
	if(pz_ReadBuf)
		delete[] pz_ReadBuf;
	if(pz_Request)
		free(pz_Request);
}

//**************************************************************************************************
void RequestHandler::EndRequest()
{
//	WriteAccessLog();
	shutdown(i_ClientSock,2);
	close(i_ClientSock);
}

//**************************************************************************************************
bool RequestHandler::ReadRequest()
{
	int iRead = 0;
	string sRequest;

	while (!b_FullyReadSockData)
	{
		bzero(z_ReadBuf, MAX_READ_BUF_LEN + 1);
		iRead = ReadSockData();
		if (iRead < 0)
		{
			WriteErrorLog("", strerror(errno));
			printf("Error: %s\n", strerror(errno));
		}
		if (iRead < MAX_READ_BUF_LEN)
			b_FullyReadSockData = true;
		if (iRead > 0)
		{
			sRequest.append(z_ReadBuf, iRead);
			//write in the error log
		}
		// iRead should be handled for errors
	}
	b_FullyReadSockData = false;

	if (sRequest.length() <= 0)
	{
		WriteErrorLog("", "Invalid Reguest");
		WriteSockData(INVALID_REQUEST, strlen(INVALID_REQUEST));
		return false;
	}

	pz_Request = strdup(sRequest.c_str());
	printf("Request Read: %s\n", sRequest.c_str());
	WriteAccessLog();
	return true;
}


//**************************************************************************************************
bool RequestHandler::IsTicket()
{
	if (strcmp(pz_Request, "ticket") == 0)
		return true;
	return false;
}

//**************************************************************************************************
bool RequestHandler::IsAbortRequest()
{
	if (strncmp(pz_Request, "mpdkilljob", 10) == 0)
		return true;
	return false;
}

//**************************************************************************************************
int RequestHandler::GetTicket()
{
	int iTicket;
	char *pzTemp = strdup(pz_Request);
	char *pzPos = strstr(pzTemp, "-a");
	iTicket = atoi(pzPos + 3);
	delete []pzTemp;
	return iTicket;
}

//**************************************************************************************************
int RequestHandler::ReadSockData()
{
#ifdef _WIN32
	return( recv(i_ClientSock, z_ReadBuf, MAX_READ_BUF_LEN, 0)); // in error SOCKET_ERROR
#else
	return( read(i_ClientSock, z_ReadBuf, MAX_READ_BUF_LEN));
#endif
}

//**************************************************************************************************
int RequestHandler::WriteSockData(char *pzBuf, int iLen)
{
#ifdef _WIN32
	return( send(i_ClientSock, pzBuf, iLen, 0));
#else
	return( write(i_ClientSock, pzBuf, iLen));
#endif
}

//**************************************************************************************************
bool RequestHandler::ProcessRequest()
{
	bool bAbort = false;
	if (IsAbortRequest())
	{
		bAbort = true;
	}
		int iNoParams;
		char **pStrings = NULL;
		int fdStdoutPipe[2];
		int iChildPID, iExecvp = 100;
		string sData;
		int t = 0;
		char szBuffer[BUF_SIZE + 1];

	if (!bAbort)
	{

//		setbuf(stdout, (char *) 0);

		if (pipe(fdStdoutPipe) < 0)
		{
			WriteErrorLog("", strerror(errno));
			printf("Error: %s\n", strerror(errno));
			return false;
		}

		int iSize = 0;
		char *pzCurrStr;
		int iNoStr = 2;
		pStrings = (char **)realloc(pStrings, sizeof(char*) * iNoStr);
//		pStrings[0] = strdup("mpiexec");
		pStrings[0] = strdup("mpdrun");
		pStrings[1] = strdup("-n");

		bool bNxtAlias = false;
		bool bAliasFound = false;
		bool bMachinefileAdded = false;
		char *pzMachineFile;
		int iNodes;
		pzCurrStr = strtok(pz_Request," ");
		if (pzCurrStr != NULL)
			iNodes = atoi(pzCurrStr);
		while (pzCurrStr != NULL)
		{
			if (pStrings == NULL)
			{
				printf("Error: %s\n", strerror(errno));
				exit(0);
			}
			if (bNxtAlias)
			{
				pzMachineFile = GetMachineFileName(pzCurrStr, iNodes);
				bNxtAlias = false;
				bAliasFound = true;
			}
			else if ((strcmp(pzCurrStr, "-a") == 0) && !bAliasFound)
				bNxtAlias = true;
			else if (bAliasFound && !bMachinefileAdded)
			{
				iNoStr += 2;
				pStrings = (char **)realloc(pStrings, sizeof(char*) * iNoStr);
				pStrings[iNoStr - 2] = strdup("-hf");
				pStrings[iNoStr - 1] = strdup(pzMachineFile);
				bMachinefileAdded = true;
			}

			iNoStr++;
			pStrings = (char **)realloc(pStrings, sizeof(char*) * iNoStr);
			pStrings[iNoStr - 1] = strdup(pzCurrStr);
			pzCurrStr = strtok (NULL, " ");
		}
		pStrings = (char **)realloc(pStrings, sizeof(char*) * (iNoStr + 1));
		pStrings[iNoStr] = (char *)0;
		pzReferenceID = strdup(pStrings[4]);
		iNoParams = iNoStr;
	}
	else
	{
		pStrings = new char*[4];
		char *pzCurrStr;
		int x = 0;
		pzCurrStr = strtok(pz_Request," ");
		while (pzCurrStr != NULL)
		{
			pStrings[x] = strdup(pzCurrStr);
			pzCurrStr = strtok (NULL, " ");
			x++;
		}
		pStrings[3] = (char *)0;
		iNoParams = 3;
	}

	char **args = pStrings;

	int nOutRead = 1;
	if ((iChildPID = fork()) < 0)
	{
		WriteErrorLog("", strerror(errno));
		printf("Error: %s\n", strerror(errno));
		return false;
	}
	else if (iChildPID == 0) //In the Child
	{
		if (!bAbort)
		{
			close(fdStdoutPipe[READ_HANDLE]);
			if(dup2(fdStdoutPipe[WRITE_HANDLE], fileno(stdout)) == -1)
			{
				WriteErrorLog("", strerror(errno));
				printf("In the Child :PID : %d : %s\n", iChildPID, strerror(errno));
				return false;
			}
			close(fdStdoutPipe[WRITE_HANDLE]);
		}

		iExecvp = execvp(*args, args);
		if (iExecvp == -1)
		{
			WriteErrorLog("", strerror(errno));
			printf("In the Child :Error : %s\n", strerror(errno));
		}
//		printf("After execute beep: iExecvp = %d\n", iExecvp); // this will not be printed.
	}
	else //in the main process
	{
		for (int i = 0; i < (t + iNoParams); i++)
		{
			free(args[i]);
		}
		free(args);

		if (!bAbort)
		{
			if (close(fdStdoutPipe[1]) == 0)
			{
				int loops = 0;
				while   (nOutRead > 0)
				{
					bzero(szBuffer, BUF_SIZE + 1);
					nOutRead = read(fdStdoutPipe[READ_HANDLE], szBuffer, BUF_SIZE);
					if (nOutRead < 0)
					{
						WriteErrorLog("", strerror(errno));
						printf("Error: %s\n", strerror(errno));
					}
					loops++;
					if(nOutRead > 0)
						sData.append(szBuffer, strlen(szBuffer));
 				}
			}
		}
	}
	if (!bAbort)
	{
		if (sData.empty())
			sData.append("error,job killed");
		printf("Data: \n%s\n", sData.c_str());

		if (WriteSockData((char *)sData.c_str(), strlen(sData.c_str())) > 0)
		{
			return true;
		}
		else
		{
	//		printf("Error: %s\n", strerror(errno));
			WriteErrorLog("", strerror(errno));
			return false;
		}
	}
	return true;
}

//**************************************************************************************************
void RequestHandler::WriteErrorLog( char *pzLevel, char *pzMSG )
{
	char	dateBuf[30];
	struct 	tm *timePtr;
	time_t	clock;

	WriteErrorLog("", strerror(errno));
	if (f_ErrorLog == NULL)
		return;
	clock = time(NULL);
	timePtr = localtime(&clock);
	strftime(dateBuf, 30, "%a %b %d %T %Y",  timePtr);
	if (*z_ClientAddr != 0)
	{
		fprintf(f_ErrorLog, "%s:%s [%s] [client %s] %s\n",__FILE__, __LINE__, dateBuf, z_ClientAddr, pzMSG);
		fflush(f_AccessLog);
	}
	else
	{
		fprintf(f_ErrorLog, "%s:%s [%s] %s\n",__FILE__, __LINE__, dateBuf, pzMSG);
		fflush(f_AccessLog);
	}
}

//**************************************************************************************************
void RequestHandler::WriteAccessLog()
{
	char	dateBuf[30];
	struct 	tm *timePtr;
	time_t	clock;

	if (f_AccessLog == NULL)
		return;
	clock = time(NULL);
	timePtr = localtime(&clock);
	strftime(dateBuf, 30, "%d/%b/%Y:%T %Z",  timePtr);
	fprintf(f_AccessLog, "%s:%d - - [%s] - %s\n", z_ClientAddr, i_ClientSock, dateBuf, pz_Request);
	fflush(f_AccessLog);
}

// **************************************************************************************************
char **RequestHandler::StrTokenizer(char *pzStr, int &iNoStr)
{
	int iSize = 0;
	char *pzCurrStr;
	char **pStrings = NULL;
	iNoStr = 0;

	pzCurrStr = strtok(pzStr," ");
	while (pzCurrStr != NULL)
	{
		iNoStr++;
		if(iNoStr > iSize)
		{
			pStrings = (char **)realloc(pStrings, sizeof(char*) * iNoStr);
			if (pStrings == NULL)
			{
				printf("Error: %s\n", strerror(errno));
				exit(0);
			}

		}
		pStrings[iNoStr - 1] = strdup(pzCurrStr);
		pzCurrStr = strtok (NULL, " ");
	}

	return pStrings;
}

// **************************************************************************************************
char *RequestHandler::GetMachineFileName(char *pzAlias, int iNodes)
{
	pthread_mutex_lock(&mutex_FileLock);

	string sFile;
	char zLine[20];
	FILE *fpHosts = fopen("/home/cig4/mpd.hosts", "r+");
	char *pzResult = NULL;
	int n = 0;
	int iPos = 0;

	string sHFName("/home/cig4/mpich2-install/bin/hf");
	string sHF;
	sHFName += pzAlias;
	FILE *fpHF = fopen(sHFName.c_str(), "w");

	pzResult = fgets(zLine, 20, fpHosts);
	while (pzResult != NULL)
	{
		if (n < iNodes)
		{
			sFile.append(zLine);
			sHF.append(zLine, strlen(zLine) - 3);
			sHF.append("\n");
			if (n == iNodes - 1)
			{
				fputs(sHF.c_str(), fpHF);
				fclose(fpHF);
			}
		}
		else
		{
			sFile.insert(iPos, zLine);
			iPos += strlen(zLine);
		}
		n++;
		pzResult = fgets(zLine, 20, fpHosts);
	}
	if (n < iNodes)
	{
		fputs(sHF.c_str(), fpHF);
		fclose(fpHF);
	}
	fseek(fpHosts, 0, SEEK_SET);
	fputs(sFile.c_str(), fpHosts);
	fclose(fpHosts);
	pthread_mutex_unlock(&mutex_FileLock);
	return strdup(sHFName.c_str());
}

// **************************************************************************************************
