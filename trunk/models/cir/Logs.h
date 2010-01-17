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

#ifndef Logs_H_
#define Logs_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "duration.h"

using namespace std;

class Logs {


private:
	char *psIPAddress;
	char pzFileName[250];
	char pzErrorMsg[1024];
	Duration *poDuration;
	ofstream *fLogs;

public:
	Logs(char *czLogType);
	Logs();
	void WriteLog(char *sError);
	void WriteLog(double dValue);
	int bIsMsgFull(int iLen);
	void writeTuning(double *dRatio, char **IPList, int size);
	char *readLog(char *cpFileDestination);
	vector<string> *tokenizer(string cpBuffer, char cDelim); 
	bool checkFileExists(string fileName);
	virtual ~Logs();
};

#endif /* Logs_H_ */
