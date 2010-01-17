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

#include <stdio.h>
#include "duration.h"
#include "Vasicek.h"
#include "def.h"

using namespace std;

int i_Rank;

int main(int argc, char* argv[])
{
	Duration d;
	d.setStart();

	Vasicek oVasicek(argc, argv, DATABASE, IP, USER, PWORD, DATACOLUMN);
	
	double* params = oVasicek.getParams();
	if (params != 0)
	{
		double* predicts = oVasicek.getPredicts(params);
	
		d.setEnd();
		double dPassedTime = d.getPassedTime();
	
		if (i_Rank == 0)
		{
			printf("Params,Intial value=%f,Mean=%f,Sigma=%f,Lambda=%f,Time=%f:Values", params[0], params[1], params[2], params[3],dPassedTime);
			fflush(stdout);
		}
	
	
		if (i_Rank == 0)
		{
			for (int i = 0; i < oVasicek.i_Forecasts; i++)
			{
				printf(",%f",predicts[i]);
				fflush(stdout);
			}
		}
	}
	return 0;
}
