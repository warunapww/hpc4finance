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
// // FILE     : TestHPC4FServer.cpp
// // AUTHOR   : Waruna Ranasinghe (warunapww@gmail.com)
// // DESC     : The implementation of the HPC4FServer is done here.
// // TODO     : 
// // HISTORY  : Date of Creation: 5-Oct-2008
//               Modified: 19-Jan-2009 : added signal handling
// //
*/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "HPC4FServer.h"

FILE *fpA;
FILE *fpE;

void finalize(int iParam)
{
	printf("\nHPC4FServer Shutting Down Sequence is started....\nClosing files....\n");
	fclose(fpA);
	fclose(fpE);
	printf("BYE!\n");
	fflush(stdout);
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, &finalize);

	int iPort = 8080;
	char *pzHost = NULL;
	int iOption;
	int iErrFlag = 0;
	int c;

	/* It is possible to run the server without specifying the Host or Port
	 * So that the default Host = localhost, Port = 8080.
	 * It is also possible to specify the Host and the port when starts 
	 * the server by using options "-h <host_ip> -p <port>" 
	 * */

	while ( (iOption = getopt(argc, argv, "h:p:")) != -1 )
	{
		switch (iOption) 
		{
		case 'h':
			pzHost = optarg;
			break;
		case 'p':
			iPort = atoi(optarg);
			break;
		default:
			iErrFlag++;
		}
	}

	if (iErrFlag)
	{
		printf("Usage: %s [-h <host ip>] [-p <port>]\n", argv[0]);
		exit (1);
	}

	struct timeval stTimeout;
	enum eSelectStatus eResult;
	
	/* Initiate the server with Host and the listening port */
	fpA = fopen("access.log", "a");
	fpE = fopen("error.log", "a");
	if (fpA == NULL)
	{
		printf("ERROR FPA: %s\n", strerror(errno));
	}
	if (fpE == NULL)
	{
		printf("ERROR FPE: %s\n", strerror(errno));
	}
	HPC4FServer server(pzHost, iPort, fpA , fpE);
//	server.SetAccessLog(fp);
	if (!server.CreateHPC4FServer())
	{
		printf("Error Creating the HPC4FServer!\n");
		return -1;
	}

	printf("Welcome to HPC4FServer! Listening on port %d\n", iPort);

//  	server.SetAccessLog(stdout);
//  	server.SetErrorLog(stdout);

	while(true)
	{
	/* Timeout value has to be set over and over again,
	 * since in Linux systems timeout value may be modified
	 * during the "select" call
	 * */

		stTimeout.tv_sec = 0;
		stTimeout.tv_usec = 50;
		eResult = server.GetConnection(&stTimeout);
		if (eResult == TIMEOUT)
		{
			continue;
		}
		if (eResult < ERR)
		{
			printf("Error ... \n");
			continue;
		}
	}

	return 0;
}

