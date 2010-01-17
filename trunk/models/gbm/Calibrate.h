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
//#include <vector.h>

class Calibrate
{
private:
	int i_Rank;
	int i_Size;
	int i_Length;
	char* pz_TableName;
	char* pz_DBName;
	char* pz_IPAddress;
	char* pz_UserName;
	char* pz_Password;
	char* pz_DataColumn;
	double* calcParamInSingleMachine();
	double* calcParamFromCentralDB();
	double* calcParamReplicatedDB();
//	double* paramCalculation(vector<double> *data);
//	double parallelMGR(vector<double> *data);
//	double parallelVariance(vector<double> *data, double mean);
	double* paramCalculation(double *data, int iSize);
	double parallelMGR(double *data, int iSize);
	double parallelVariance(double *data, double mean, int iSize);

public:
	Calibrate(char* pzDBName, char* pzIP, char* pzUserName, char* pzPW, char* pzTName, char* pzDataColumn, int iRank, int iSize);
	double* calcParameters();
	bool checkTable();
	~Calibrate(void);
};
