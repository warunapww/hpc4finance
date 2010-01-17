#include <stdio.h>
#include <memory.h>
#include <errno.h>
#include <string.h>
#define bzero(a, b) memset((a),0,(b))
#if defined(_WIN32)
#include <winsock2.h>
#include <io.h>
//#include <stdarg.h>
#define snprintf _snprintf
#define bzero(a, b) memset((a),0,(b))
#define index strchr
#define rindex strrchr
#define SIGPIPE SIGABRT
#else
#include <unistd.h>
#include <sys/file.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h> //atoi
#include <pthread.h>
#endif
#include <string>

using namespace std;
 
#define BUF_SIZE 102400

int WriteSockData(int iSock, char *pzBuf, int iLen);
bool WinSockDLLVersionOK();
int ReadSockData(int iSock, char *pzBuf, int iLen);
void *ThreadF(void *args);

char *gHost = "10.8.102.27";//NULL;
int gPort = 8080;
struct  sockaddr_in addr;
int argcs;
char **argvs;

int main(int argc, char **argv)
{
	argcs = argc;
	argvs = argv;
#define THREADS iNoThreads
	int iNoThreads = 1;


	printf("Host: %s\tPort: %d\n", gHost, gPort);
/*	
	if (argc < 2)
	{
		gHost = strdup("127.0.0.1");///"10.8.102.27";//"127.0.0.1";//NULL;
		port = 8080;//4045; //8085;
	}
	else if (argc == 3)
	{
		gHost = argv[1];
		gPort = atoi(argv[2]);
	}
	else
		printf("Usage: %s <host> <gPort>\n", argv[0]);
*/

#ifdef _WIN32
	if(!WinSockDLLVersionOK())
	{
		printf("WINSOCK ERROR\n");
		return 0;
	}
#endif
	int jj = atoi(argv[1]);
	int i = 0;
	pthread_t thread[THREADS];
//	while(i++ < 100)
	while (true)
	{
		if (i == THREADS)
		{
			break;
// 			char *s = "hello world";
// 			*s = 'H';
//			return 0;
		}
// //			abort();
//
		if (pthread_create(&thread[i], NULL, ThreadF, (void *)jj) != 0)
        	{
                //	WriteErrorLog("", strerror(errno)); // CHK whether errno set when there is a thread error
                	printf("Error:%d: %s\n", i, strerror(errno));
        	 //       return ERR;//EXIT_FAILURE;
	        }
		i++;
	}
	
	for (int j = 0; j < THREADS; j++)
	{
		pthread_join(thread[j], NULL);
	}
	return 0;
}

//**************************************************************************************************
void *ThreadF(void *args)
{	
		char *pzReqq = new char[100];
		char *pzTicket = "ticket";
		char pzAbort[100];
		sprintf(pzAbort, "mpdkilljob -a %d", (int)args);
	string sReq;
	char *pzSpace = " ";
	for (int i = 1; i < argcs - 1; i++)
	{
		sReq.append(argvs[i]);
		if (argcs != i + 1)
			sReq.append(pzSpace);
	}
	int iResult = 0;
	char *pzBuf = new char[BUF_SIZE + 1];
	bzero(pzBuf, 101);

	int iSock = socket(AF_INET, SOCK_STREAM, 0);
	if (iSock == -1)
	{
		printf("Error Socket");
		exit (0);
//		return 0;
	}
	
// 	//Making it a blocking socket
// 	u_long nNoBlock = 1;
// 	ioctlsocket(iSock, FIONBIO, &nNoBlock);


//#ifdef SO_REUSEADDR
	int opt = 1;
	setsockopt(iSock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,sizeof(int));
//#endif
	
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	if (gHost == NULL)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(gHost);
	}
	addr.sin_port = htons((u_short)gPort);


	if(connect(iSock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
//		int err = WSAGetLastError();
		printf("Error: %s \n", strerror(errno));
//		WSAEADDRNOTAVAIL
//		return 0;
		exit(0);
	}

//		WriteSockData(iSock, "Hello Server...", 31);
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm", 26);
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm 100 .15 .1", strlen("/home/cig4/Desktop/GBM/gbm 100 .15 .1"));
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm -t ibm", strlen("/home/cig4/Desktop/GBM/gbm -t ibm"));
//	WriteSockData(iSock, "/mnt/models/gbm -t ibm", strlen("/mnt/models/gbm -t ibm"));
//	WriteSockData(iSock, (char*)sReq.c_str(), sReq.size());
	WriteSockData(iSock, pzAbort, strlen(pzAbort));
//	WriteSockData(iSock, "/home/cig4/mpich2-install/examples/cpi", strlen("/home/cig4/mpich2-install/examples/cpi"));
	printf("Data Abort Request sent to Server : %d \n", (int)args );
	fflush(stdout);

/*	fd_set fdSet;
	struct	timeval stTimeout;

	stTimeout.tv_sec = 1000;
	stTimeout.tv_usec = 50000;
	
	FD_ZERO(&fdSet);
	FD_SET((unsigned)iSock, &fdSet);
	iResult = 0;

	int yy = 0;
	while(iResult == 0)
	{
		yy++;
		iResult = select(iSock + 1, &fdSet, 0, 0, &stTimeout);
		if (iResult < 0)
			exit (0);//return -1;
		if (&stTimeout != 0 && iResult == 0)
			iResult = 0;
		if (iResult > 0)
			break;

		printf("Waiting Time out: %d\n", yy);
	}
	
	int len = ReadSockData(iSock, pzBuf, BUF_SIZE);
	int iAlias = atoi(pzBuf);
		sprintf(pzReqq, "3 -a %d /mnt/models/gbm 100 .5 .15 -m 10 -n 10000", iAlias);

	if (len > 0)
	{
		printf("Read from server:%d\n%s\n", (int)args, pzBuf);
		
	}
	else
		printf("Error read msg length : %d <= 0\n", len);
	
	iSock = socket(AF_INET, SOCK_STREAM, 0);
	if (iSock == -1)
	{
		printf("Error Socket");
		exit (0);
//		return 0;
	}
	if(connect(iSock, (struct sockaddr *)&addr, sizeof(addr)) != 0)
	{
//		int err = WSAGetLastError();
		printf("Error: %s \n", strerror(errno));
//		WSAEADDRNOTAVAIL
//		return 0;
		exit(0);
	}

//		WriteSockData(iSock, "Hello Server...", 31);
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm", 26);
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm 100 .15 .1", strlen("/home/cig4/Desktop/GBM/gbm 100 .15 .1"));
//	WriteSockData(iSock, "/home/cig4/Desktop/GBM/gbm -t ibm", strlen("/home/cig4/Desktop/GBM/gbm -t ibm"));
//	WriteSockData(iSock, "/mnt/models/gbm -t ibm", strlen("/mnt/models/gbm -t ibm"));
//	WriteSockData(iSock, (char*)sReq.c_str(), sReq.size());
	WriteSockData(iSock, pzReqq, strlen(pzReqq));
//	WriteSockData(iSock, "/home/cig4/mpich2-install/examples/cpi", strlen("/home/cig4/mpich2-install/examples/cpi"));
	printf("Data sent to Server : %d \n", (int)args );
	fflush(stdout);

	//fd_set fdSet;
	//struct	timeval stTimeout;

	stTimeout.tv_sec = 1000;
	stTimeout.tv_usec = 50000;
	
	FD_ZERO(&fdSet);
	FD_SET((unsigned)iSock, &fdSet);
	iResult = 0;

	yy = 0;
	while(iResult == 0)
	{
		yy++;
		iResult = select(iSock + 1, &fdSet, 0, 0, &stTimeout);
		if (iResult < 0)
			exit (0);//return -1;
		if (&stTimeout != 0 && iResult == 0)
			iResult = 0;
		if (iResult > 0)
			break;

		printf("Waiting Time out: %d\n", yy);
	}
	
	len = ReadSockData(iSock, pzBuf, BUF_SIZE);

	if (len > 0)
		printf("Read from server:%d\n%s\n", (int)args, pzBuf);
	else
		printf("Error read msg length : %d <= 0\n", len);

*/
	shutdown(iSock,2);
	close(iSock);
}

//**************************************************************************************************
int WriteSockData(int iSock, char *pzBuf, int iLen)
{
#ifdef _WIN32
	return( send(iSock, pzBuf, iLen, 0));
#else
	return( write(iSock, pzBuf, iLen));
#endif
}

//**************************************************************************************************
int ReadSockData(int iSock, char *pzBuf, int iLen)
{
#ifdef _WIN32
	{
		int iCount = recv(iSock, pzBuf, iLen, 0);
		return iCount;
	}
	//	return( recv(i_ClientSock, z_ReadBuf, MAX_READ_BUF_LEN, 0)); // in error SOCKET_ERROR
#else
	return( read(iSock, pzBuf, iLen));
#endif
}

#ifdef _WIN32
//**************************************************************************************************
bool WinSockDLLVersionOK()
{
	WORD 	wVersionRequested;
	WSADATA wsaData;
	int 	iErr;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	iErr = WSAStartup( wVersionRequested, &wsaData );
	
	/* Found a usable winsock dll? */
	if( iErr != 0 ) 
		return false;
	
		/* 
		** Confirm that the WinSock DLL supports 2.2.
		** Note that if the DLL supports versions greater 
		** than 2.2 in addition to 2.2, it will still return
		** 2.2 in wVersion since that is the version we
		** requested.
	*/
	
	if( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 ) 
	{
		
	/* 
	** Tell the user that we could not find a usable
	** WinSock DLL.
		*/
		WSACleanup( );
		return false;
	}
	
	printf("Winsock DLL is ok\n");
	/* The WinSock DLL is acceptable. Proceed. */
	return true;
}
#endif
//**************************************************************************************************
