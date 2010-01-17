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

#include "Logs.h"

class Vasicek
{
private:
	int i_ArgCount;
	char** pz_Args;
	char* pz_DBName;
	char* pz_IPAddress;
	char* pz_UserName;
	char* pz_Password;
	char* pz_DataColumn;
	char* pz_TableName;
	int i_Size;
	double d_TimeStep;
	int i_Iterations;
	bool validateParams(int argc,char* args[]);// parameter validation
	Logs *poTansLog;
	int getNthPrime(int n);

public:
	Vasicek(int argCount, char* args[], char* pzDBName, char* pzIP, char* pzUserName, char* pzPW, char* pzDataColumn);
	
	int i_Forecasts;
	double* getParams();
	double* getPredicts(double *dParams);
	~Vasicek(void);
};
