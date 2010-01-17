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

#pragma once
//#include <vector>

using namespace std;

class DataExtracter
{
private:
	int i_ResultLength;
	char* pz_TableName;
	char* pz_DBName;
	char* pz_UserName;
	char* pz_Password;
	char* pz_IPAddress;

public:
	DataExtracter(char* pzDName, char* pzUserName, char* pzPW, char* pzTName, char* pzIP);
//	vector<double> *getData(char *coloumName);
//	vector<double> *getDataParellel(int iStartingRow, int iEndingRow, char *coloumName);
	double *getData(char *coloumName, int &iLen);
	double *getDataParellel(int iStartingRow, int iEndingRow, char *coloumName);
	int getDuration(void);
	int getNoOfRows(void);
	double getLastData(char* coloumName);
	//int getDurationParallel( int iStartingRow, int iEndingRow );
//	vector<double> *getReplicatedDataParellel( int iStartingRow, int iEndingRow, char *coloumName );
	double *getReplicatedDataParellel( int iStartingRow, int iEndingRow, char *coloumName );
	//int getReplicatedDurationParallel( int iStartingRow, int iEndingRow );
	void setDBProperties(char* dbName, char* username, char* password, char* tablename, char* ip );
	
	int I_ResultLength() const { return i_ResultLength; }
	void I_ResultLength(int val) { i_ResultLength = val; }	

public:
	~DataExtracter(void);
};
