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
// // FILE     : HPC4FServer.h
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC    : The header file defines the HPC4FServer class, its properties
//               and methods. This is satisfactorily tested with UNIX
//               environment while partially implemented for WINDOWS.
// // TODO     : Failover and Load Balancing
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 24-Nov-2008: tpool.h
//               Modified: 19-Jan-2009: #include <map>, pz_ErrorMSG, pmap_Tickets
// //
*/

#ifndef _HPC4FSERVER_H_
#define _HPC4FSERVER_H_

#include "defs.h"

#ifdef _WIN32
#include <winsock2.h>
#include <io.h>
#else
#include <unistd.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <stdlib.h>
#include "tpool.h" 
#endif

#include <map>

using namespace std;

enum eSelectStatus
{
	ERR = -1,
	TIMEOUT = 0,
	PENDING_REQUEST = 1
};

class HPC4FServer
{
private:
	int i_ServerSock;
	int i_Port;
	char z_ClientAddr[IP_ADDR_LEN];
	char *pz_HostAddr;
	FILE *f_ErrorLog;
	FILE *f_AccessLog;
	char *pz_ErrorMSG;

	map<int, char*>	*pmap_Tickets;

	Tpool *tpool;

	void WriteErrorLog(char *pzLevel, char *pzMSG);
	void WriteAccessLog();
#ifdef _WIN32
	bool WinSockDLLVersionOK();
#endif
public:
	HPC4FServer();
	HPC4FServer(char *pzHost, int iPort);
	HPC4FServer( char *pzHost, int iPort, FILE *fAccessLog, FILE *fErrorLog);
	virtual ~HPC4FServer();

	bool CreateHPC4FServer();
	enum eSelectStatus GetConnection(struct timeval *stTimeout);

	bool SetErrorLog(FILE *fp); // this should call before call 'GetConnection'
	bool SetAccessLog(FILE *fp);// this should call before call 'GetConnection'
};

#endif // _HPC4FSERVER_H_

