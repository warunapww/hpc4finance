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

#include <iostream>
#include <fstream>
//#include <sys/unistd.h>
//#include <sys/socket.h>
#include <vector>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <exception>
#include "def.h"
#include "Logs.h"
#include "duration.h"

using namespace std;

Logs::Logs(char *czLogType) {

	char psIPAddress[128];
	gethostname(psIPAddress, sizeof psIPAddress);
	
	sprintf(pzFileName , "%s/%s/%s.txt", LOGLOCATION ,czLogType, psIPAddress);
	poDuration = new Duration();
	fLogs = new ofstream(pzFileName, ios::app);
	
}

Logs::Logs() {
	
}

void Logs::WriteLog(char *sError)
{
	char *pzMsg = strdup(sError);
	int iMsgLen = strlen(pzMsg);

	int isMsgFul = bIsMsgFull(iMsgLen);
	

	if(isMsgFul == 1)
	{
		fLogs->write(pzErrorMsg, strlen(pzErrorMsg));
		pzErrorMsg[0] = (char)NULL;
	}
	else if(isMsgFul == 2)
	{
		strcat(pzErrorMsg,pzMsg);
		fLogs->write(pzErrorMsg, strlen(pzErrorMsg));
		pzErrorMsg[0] = (char)NULL;
	}
	else
	{
		strcat(pzErrorMsg,pzMsg);
	}
}



Logs::~Logs() {
	// TODO Auto-generated destructor stub
	fLogs->write(pzErrorMsg, strlen(pzErrorMsg));
	fLogs->close();
}

int Logs::bIsMsgFull(int iLen)
{
	int iSize = strlen(pzErrorMsg)+ iLen;
	if(iSize > 1024)
	{
		return 1;
	}
	else if(iSize == 1024)
	{
		return 2;
	}

	return 3;
}

void Logs::WriteLog(double dValue)
{
	char czValue[32];
	sprintf(czValue,", %f",dValue);
	WriteLog(czValue);
}

void Logs::writeTuning(double *dRatio, char **IPList, int size)
{
	char pzTuningFileName[256];
	try {
		sprintf(pzTuningFileName , "%s/tuning.txt", LOGLOCATION);
		ofstream *fTuning = new ofstream(pzTuningFileName, ios::out);
	
		for(int i = 0; i < size; i++)
		{
			char cTempValue[150];
			
			
			if(i == size -1 )
			{
				sprintf(cTempValue,"%s %f", IPList[i] , dRatio[i] );			
			}
			else
			{
				sprintf(cTempValue,"%s %f;", IPList[i] , dRatio[i] );
			}
			
			//printf("%d:%s",i,IPList[i] );
			//fflush(stdout);
			
			fTuning->write( cTempValue, strlen( cTempValue ));
			//strcat(cTuningValue,cTempValue);
		}
		fTuning->close();
	} 
	catch( const std::exception& e )
	{
		printf("%s", e.what());
		
		return;
	}
	
}

char* Logs::readLog(char *cpFileDestination)
{
	char *cpBuffer;
	int iLength;
	//pzFileName = strdup( cpFileDestination );
	sprintf(pzFileName , "%s/%s", LOGLOCATION, cpFileDestination );
	
	ifstream *fReading = new ifstream(pzFileName, ios::binary );
	
	fReading->seekg (0, ios::end);
	iLength = fReading->tellg();
  	fReading->seekg (0, ios::beg);
  	
  	cpBuffer = new char [iLength];
	
	fReading->read (cpBuffer,iLength);
  	fReading->close();	
	//printf("%s", cpBuffer );
	
	return cpBuffer; 
}

vector<string> *Logs::tokenizer(string cpBuffer, char cDelim)
{
	string sBuffer = cpBuffer;
	
	vector<string> *psTokens = new vector<string>;
	
	int lastPos=0, i=0;
	// Find first "non-delimiter".
	string::size_type pos  = sBuffer.find_first_of(cDelim,lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		psTokens->push_back(sBuffer.substr(lastPos, pos - lastPos));
		
		lastPos = sBuffer.find_first_not_of(cDelim, pos);
		pos = sBuffer.find_first_of(cDelim, lastPos);
		i++;
	}
	
	return psTokens;
}

bool Logs::checkFileExists(string fileName)
{
	bool bFileExists = true;
	fstream inp;
	//ofstream out;
	sprintf(pzFileName , "%s/%s", LOGLOCATION ,fileName.c_str());

	inp.open(pzFileName, ifstream::in);
	
	//inp.close();
	if(inp.fail())
	{
		inp.clear(ios::failbit);
		bFileExists = false;
	}
	
	return bFileExists;
	
}
