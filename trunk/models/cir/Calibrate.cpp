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

#include <mpi.h>

#ifdef WIN32
#include <windows.h>
#include <winsock.h>
#pragma warning (disable: 4514 4786)
#pragma warning( push, 3 )
#endif

#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
//#include <vector>
#include "Calibrate.h"
#include "DataExtracter.h"
#include <math.h>
#include <string.h>
#include "def.h"

using namespace std;

Calibrate::Calibrate(char* pzDBName, char* pzIP, char* pzUserName, char* pzPW, char* pzTName, char* pzDataColumn, int i_Rank, int i_Size, double dTimeStep)
{
	pz_DBName = strdup(pzDBName);
	pz_UserName = strdup(pzUserName);
	pz_Password = strdup(pzPW);
	pz_TableName = strdup(pzTName);
	pz_IPAddress = strdup(pzIP);
	pz_DataColumn = strdup(pzDataColumn);
	i_Rank = i_Rank;
	i_Size = i_Size;
	d_TimeStep = dTimeStep;
	pd_Data = NULL;
	i_Len = 0;
}

Calibrate::~Calibrate(void)
{
	if (pz_DBName)
		delete pz_DBName;
	if (pz_UserName)	
		delete pz_UserName;
	if (pz_Password)
		delete pz_Password;
	if (pz_TableName)
		delete pz_TableName;
	if (pz_TableName)
		delete pz_IPAddress;
	if (pz_DataColumn)
		delete pz_DataColumn;
	if (pd_Data)
		delete []pd_Data;
}

bool Calibrate::checkTable()
{
	DataExtracter *poDataEx = new DataExtracter(pz_DBName, pz_UserName, pz_Password, pz_TableName, pz_IPAddress);
	
	bool status = true;
	i_Length = poDataEx->getNoOfRows(); 
	if(i_Length == 0)
	{
		status = false;;
	}
	delete poDataEx;
	return status;
}

double* Calibrate::calcParameters()
{
	double *pdCalibValues = calcParamInSingleMachine(); //TODO: perform size check of table to decide on parallel calibration
	
	return pdCalibValues;
}

double* Calibrate::calcParamReplicatedDB()
{
	MPI_Status mpiStatus;
	int bIsSuccesl = 0;
	double *pdCalibParams = new double[4];
	
	int iTag =1;
	
	double dSx, dSy, dSxx, dSyy, dSxy;
	double mu, lambda, sigma_square, sigma;
	double partialParameters[5];
//	printf("Para calib1\n");
//	fflush(stdout);
	DataExtracter *poDataEx	= new DataExtracter(pz_DBName, pz_UserName, pz_Password, pz_TableName, pz_IPAddress);

	int iNoOfRows = i_Length / i_Size;

	int iInitialRow = iNoOfRows * i_Rank + 1;
	int iFinalRow;

	if( i_Rank == i_Size - 1)
	{
		iFinalRow = i_Length;
	}
	else
	{
		iFinalRow = iNoOfRows * (i_Rank + 1);
	}	
	
	pd_Data = poDataEx->getReplicatedDataParellel(iInitialRow, iFinalRow, pz_DataColumn);	
	i_Len = iFinalRow - iInitialRow + 1;
	partialParameters[0] = getSxPara();
	partialParameters[1] = getSyPara();
	partialParameters[2] = getSxxPara();
	partialParameters[3] = getSyyPara();
	partialParameters[4] = getSxyPara();
	if(i_Rank != 0)
	{
		MPI_Send(&partialParameters, 5, MPI_DOUBLE, 0,iTag, MPI_COMM_WORLD);
	}
	else
	{
	//	vector<vector<double> > parametersCollection(i_Size, vector<double> (5));
		double **parametersCollection = new double*[i_Size];
		for (int i = 0; i < i_Size; i++)
		{
			parametersCollection[i] = new double[5];
		}

		double tempResults[5];

		for(int i=0; i< 5 ; i++ )
		{
			parametersCollection[0][i] = partialParameters[i];
		}

		for(int i=1; i< i_Size; i++)
		{
			MPI_Recv(&tempResults, 5, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD, &mpiStatus);

			for(int k = 0; k < 5; k++)
			{
				parametersCollection[i][k] = tempResults[k];
			}
		}

		for(int i=0; i < 5 ; i++ )
		{
			double temp = 0;
		
			for(int k=0; k < i_Size ; k++)
			{
				temp = temp + parametersCollection[k][i];
			}

			if(i == 0)
				dSx = temp;
			else if(i == 1)
				dSy = temp;
			else if(i == 2)
				dSxx = temp;
			else if(i == 3)
				dSyy = temp;
			else if(i == 4)
				dSxy = temp;
		}
		i_Length = i_Length -1;
		
		double dMu = ((dSy * dSxx) - (dSx * dSxy)) / ((i_Length * (dSxx - dSxy)) - (pow(dSx,2) - (dSx * dSy)));
		double dLambda = (-1 / d_TimeStep) * (log((dSxy - dMu * dSx - dMu * dSy + i_Length * pow(dMu,2)) / 
								(dSxx - 2 * dMu * dSx + i_Length * pow(dMu,2))));
	
		double dAlpha = exp(-1 * dLambda * d_TimeStep);
	
		double dSigmaHatSq = (dSyy - (2 * dAlpha * dSxy) + (pow(dAlpha,2) * dSxx) - (2 * dMu * (1-dAlpha) * 
					(dSy - dAlpha * dSx)) + (i_Length * pow(dMu,2) * (pow((1-dAlpha),2)))) / i_Length;
	
		double dSigmaSq = dSigmaHatSq * 2 * dLambda / (1 - pow(dAlpha,2));

		pdCalibParams[0] = poDataEx->getLastData(pz_DataColumn);
		pdCalibParams[1] = dMu;
		pdCalibParams[2] = sqrt(dSigmaSq);
		pdCalibParams[3] = dLambda;
		
		for (int i = 0; i < i_Size; i++)
		{
			delete	[]parametersCollection[i];
		}
		delete	[]parametersCollection;
		
		delete poDataEx;
	}
	MPI_Bcast ( pdCalibParams , 4, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	if (i_Rank != 0)
	{
		MPI_Send  ( &bIsSuccesl, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	if(i_Rank == 0)
	{
		for(int i = 1; i < i_Size; i++)
		{
			MPI_Recv( &bIsSuccesl , 1, MPI_INT, i, 1, MPI_COMM_WORLD, &mpiStatus);
		}
	}
	
	return pdCalibParams;
}

double* Calibrate::calcParamInSingleMachine()
{
	MPI_Status mpiStatus;
	
	double *pdCalibParams = new double[4];
	int bIsSuccesl = 0;

	if (i_Rank == 0)
	{		
		DataExtracter *poDataEx	= new DataExtracter(pz_DBName, pz_UserName, pz_Password, pz_TableName, pz_IPAddress);

		int iLen = 0;	
		pd_Data = poDataEx->getData(DATACOLUMN, iLen);
		i_Len = iLen;

//		printf("ILEN: %d", iLen);
//		fflush(stdout);

		double dSx = getSx();
		double dSy = getSy();
		double dSxx = getSxx();
		double dSyy = getSyy();
		double dSxy = getSxy();
//		printf("Esses: %f,%f,%f,%f\n", dSx, dSy, dSxx, dSyy, dSxy);
//		fflush(stdout);
		delete pd_Data;

		i_Length = i_Length -1;
		
		double dMu = ((dSy * dSxx) - (dSx * dSxy)) / ((i_Length * (dSxx - dSxy)) - (pow(dSx,2) - (dSx * dSy)));
		double dLambda = (-1 / d_TimeStep) * (log((dSxy - dMu * dSx - dMu * dSy + i_Length * pow(dMu,2)) / 
								(dSxx - 2 * dMu * dSx + i_Length * pow(dMu,2))));
	
		double dAlpha = exp(-1 * dLambda * d_TimeStep);
	
		double dSigmaHatSq = (dSyy - (2 * dAlpha * dSxy) + (pow(dAlpha,2) * dSxx) - (2 * dMu * (1-dAlpha) * 
					(dSy - dAlpha * dSx)) + (i_Length * pow(dMu,2) * (pow((1-dAlpha),2)))) / i_Length;
	
		double dSigmaSq = dSigmaHatSq * 2 * dLambda / (1 - pow(dAlpha,2));

		pdCalibParams[0] = poDataEx->getLastData(pz_DataColumn);
		pdCalibParams[1] = dMu;
		pdCalibParams[2] = sqrt(dSigmaSq);
		pdCalibParams[3] = dLambda;

		MPI_Bcast ( pdCalibParams , 4, MPI_DOUBLE , 0, MPI_COMM_WORLD );
		
		delete poDataEx;
	}
	else
	{
		MPI_Bcast ( pdCalibParams , 4, MPI_DOUBLE , 0, MPI_COMM_WORLD );
		MPI_Send  ( &bIsSuccesl, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	if(i_Rank == 0)
	{
		for(int i = 1; i < i_Size; i++)
		{
			MPI_Recv( &bIsSuccesl , 1, MPI_INT, i, 1, MPI_COMM_WORLD, &mpiStatus);
		}
	}
	
	return pdCalibParams;
}

//************************************************************************

double Calibrate::getSx()
{
	double dSx = 0;
//			printf("dSx: %d\n", i_Len);
	for (unsigned int i = 1; i < i_Len; i++)
	{
//		if (i == 1)
//			printf("dSx\n");
		dSx += pd_Data[i - 1];
	}
	return dSx;
}

//************************************************************************

double Calibrate::getSy()
{
	double dSy = 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSy += pd_Data[i];//cout << pd_Data[i] << endl;
	}
	return dSy;
}

//************************************************************************

double Calibrate::getSxx()
{
	double dSxx = 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSxx += pow(pd_Data[i - 1],2);
	}
	return dSxx;
}

//************************************************************************

double Calibrate::getSyy()
{
	double dSyy= 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSyy += pow(pd_Data[i],2);
	}
	return dSyy;
}

//************************************************************************

double Calibrate::getSxy()
{
	double sumXY = 0;
	for (unsigned int i = 1; i < i_Len; i++){
		sumXY += pd_Data[i] * pd_Data[i-1];
	}
	return sumXY;
}

//************************************************************************

double Calibrate::getSxPara()
{
	double dSx = 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSx += pd_Data[i - 1];
	}
	return dSx;
}

//************************************************************************

double Calibrate::getSyPara()
{
	double dSy = 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSy += pd_Data[i];//cout << pd_Data[i] << endl;
	}
	return dSy;
}

//************************************************************************

double Calibrate::getSxxPara()
{
	double dSxx = 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSxx += pow(pd_Data[i - 1],2);
	}
	return dSxx;
}

//************************************************************************

double Calibrate::getSyyPara()
{
	double dSyy= 0;
	for (unsigned int i = 1; i < i_Len; i++)
	{
		dSyy += pow(pd_Data[i],2);
	}
	return dSyy;
}

//************************************************************************

double Calibrate::getSxyPara()
{
	double sumXY = 0;
	for (unsigned int i = 1; i < i_Len; i++){
		sumXY += pd_Data[i] * pd_Data[i-1];
	}
	return sumXY;
}
