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
// // FILE     : RequestHandler.h 
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : The header file defines the RequestHandler class, its properties
//               and methods. This is satisfactorily tested with UNIX
//               environment while partially implemented for WINDOWS.
// // TODO     : 
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 16-Nov-2008 : 'strtokenizer' function added.
//               Modified: 14-Jan-2009 : GetTicket, IsTicket, IsAbortRequest is added
//               Modified: 15-Jan-2009 : GetMachineFileName aded for load balancing
// //
*/

#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_

#include <netdb.h>
#include <pthread.h>
#include "defs.h"

class RequestHandler
{
private:
	int i_ServerSock;
	int i_Port; 
	int i_ClientSock;
	int i_ReadBufRemain;	
	bool b_FullyReadSockData; 
	char *pz_Request;
	char *pz_ReadBuf; 
	char z_ClientAddr[IP_ADDR_LEN];
	char z_ReadBuf[MAX_READ_BUF_LEN + 1];

	FILE *f_ErrorLog;
	FILE *f_AccessLog;
	
	int ReadSockData(); 	/* read socket data to z_ReadBuf, Here if the return value is equal to the
				 * MAX_READ_BUF_LEN, then theres more to read at socket. 
				 * So b_FullyReadSockData should be false */
//	int ReadChar(char *pcChar);  // if needs to tokanize, read char by char from the z_ReadBuf
	void WriteErrorLog(char *pzLevel, char *pzMSG);
	void WriteAccessLog();

	char **StrTokenizer(char *pzStr, int &iNoStr);
public:
	RequestHandler(int iClientSock, char *pzClientIPAddr, int iPort, int iServerSock, FILE *fErrorLog, FILE *fAccessLog);
	virtual ~RequestHandler();
	
	bool ProcessRequest();
	void EndRequest();
	bool ReadRequest();	

	bool IsTicket();
	bool IsAbortRequest();
	char *GetMachineFileName(char *pzAlias, int iNodes);
	int GetTicket();
	
	int WriteSockData(char *pzBuf, int iLen); // write data to Socket

	char *pzReferenceID;
	pthread_mutex_t mutex_FileLock;	
};

#endif //_REQUESTHANDLER_H_

