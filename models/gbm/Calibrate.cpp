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

using namespace std;

Calibrate::Calibrate(char* pzDBName, char* pzIP, char* pzUserName, char* pzPW, char* pzTName, char* pzDataColumn, int iRank, int iSize)
{
	pz_DBName = strdup(pzDBName);
	pz_UserName = strdup(pzUserName);
	pz_Password = strdup(pzPW);
	pz_TableName = strdup(pzTName);
	pz_IPAddress = strdup(pzIP);
	pz_DataColumn = strdup(pzDataColumn);
	i_Rank = iRank;
	i_Size = iSize;
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
	if (pz_IPAddress)
		delete pz_IPAddress;
	if (pz_DataColumn)
		delete pz_DataColumn;
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
	else if (i_Length > 0)
	{
		i_Length = i_Length;
	}
	delete poDataEx;
	return status;
}

// Calibrate mu, sigma for GBM using hostorical data 
double* Calibrate::calcParameters()
{
	double *pdCalibValues;
	if(i_Length < 4000) 
	{
		pdCalibValues = calcParamInSingleMachine();
	}
	else
	{	
		pdCalibValues = calcParamReplicatedDB();
	}
	
	return pdCalibValues;
}


// Calibrate the mu and varience in a single machine
double* Calibrate::calcParamInSingleMachine()
{
	MPI_Status mpiStatus;
	
	double *pdCalibParams = new double[3];
	int bIsSuccesl = 0;

	if (i_Rank == 0)
	{		
		DataExtracter *poDataEx	= new DataExtracter(pz_DBName, pz_UserName, pz_Password, pz_TableName, pz_IPAddress);
	
		//vector<double> *pdData = poDataEx->getData(pz_DataColumn);
		int iLen = 0;
		double *pdData = poDataEx->getData(pz_DataColumn, iLen);
		if (iLen == 0)
		{	
			return 0;
		}
		
		double *muAndSigma = paramCalculation(pdData, iLen);

		pdCalibParams[0] = poDataEx->getLastData(pz_DataColumn);
		pdCalibParams[1] = muAndSigma[0];
		pdCalibParams[2] = muAndSigma[1];
		
		MPI_Bcast ( pdCalibParams , 2, MPI_DOUBLE , 0, MPI_COMM_WORLD );
		
		delete []muAndSigma;
		delete pdData;
		delete poDataEx;
	}
	else
	{
		MPI_Bcast ( pdCalibParams , 2, MPI_DOUBLE , 0, MPI_COMM_WORLD );
		MPI_Send  ( &bIsSuccesl, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
	}

	if(i_Rank == 0)
	{
		for(int i=1;i < i_Size ;i++)
		{
			MPI_Recv( &bIsSuccesl , 1, MPI_INT, i, 1, MPI_COMM_WORLD, &mpiStatus);
		}
	}
	
	return pdCalibParams;
}
// Calibrate the mu and varience in multiple machines using replicated datbases
double* Calibrate::calcParamReplicatedDB()
{
	MPI_Status mpiStatus;
	int iNoOfRows;

	DataExtracter *poDataEx = new DataExtracter(pz_DBName, pz_UserName, pz_Password, pz_TableName, "localhost");
	if (i_Length == 0)
		return (double *)0;

	iNoOfRows=(int)i_Length / i_Size;

	int iInitialRow = iNoOfRows * i_Rank + 1;
	int iFinalRow;

	if( i_Rank == i_Size - 1)
	{
		iFinalRow = i_Length;
	}
	else
	{
		iFinalRow = iNoOfRows * (i_Rank + 1) + 1;
	}

//	vector<double> *pdData = poDataEx->getReplicatedDataParellel(iInitialRow,iFinalRow, pz_DataColumn);
	double *pdData = poDataEx->getReplicatedDataParellel(iInitialRow,iFinalRow, pz_DataColumn);
	
	double dLocalTotMGR = parallelMGR(pdData, iFinalRow - iInitialRow + 1);
	
	double dFinalStDev, dFinalMGR;
	double dTotalVarience, dTotalMGR;
		
	if(i_Rank != 0)
	{
		MPI_Send(&dLocalTotMGR, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}	

	if(i_Rank == 0)
	{
		dTotalMGR = dLocalTotMGR;

		double dTempParam;    

		for(int i=1;i < i_Size ;i++)
		{
			MPI_Recv(&dTempParam, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &mpiStatus);
			dTotalMGR = dTotalMGR + dTempParam;		
		}
		dFinalMGR = dTotalMGR / (i_Length-1);
	}
	MPI_Bcast ( &dFinalMGR, 1, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	fflush(stdout);
	double dLocalTotVariance = parallelVariance(pdData, dFinalMGR, iFinalRow - iInitialRow + 1);
	
	if(i_Rank != 0)
	{
		MPI_Send(&dLocalTotVariance, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
	}	

	if(i_Rank == 0)
	{
		dTotalVarience = dLocalTotVariance;

		double dTempParam;    

		for(int i = 1;i < i_Size ; i++)
		{
			MPI_Recv(&dTempParam, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &mpiStatus);
			dTotalVarience += dTempParam;		
		}
		 
		double dFinalVarience = dTotalVarience / (i_Length-1);
		dFinalStDev = sqrt(dFinalVarience);
		
	}
	MPI_Bcast ( &dFinalStDev, 1, MPI_DOUBLE , 0, MPI_COMM_WORLD );

	double *resultSet = new double[3];
	
	resultSet[0] = poDataEx->getLastData(pz_DataColumn);
	resultSet[1] = dFinalMGR;
	resultSet[2] = dFinalStDev;
	
	delete pdData;
	delete poDataEx;
	return resultSet;
}

//double Calibrate::parallelVariance(vector<double> *data, double mean)
double Calibrate::parallelVariance(double *data, double mean , int iSize)
{
	double dTotalVariance = 0;
//	int iArraySize = data->size();
	int iArraySize = iSize;

	for(int i = 1; i < iArraySize ; i++ )
	{
		double tempDifference =log(data[i]) - log(data[i-1]) ; 			
		dTotalVariance += pow((tempDifference - mean),2) ;	
	}
	
	return dTotalVariance;
}

//double Calibrate::parallelMGR(vector<double> *data)
double Calibrate::parallelMGR(double *data, int iSize)
{
	double dTotalGrowthRate = 0;
	//double dTotalSigma=0;
	//double *partialParameters = new double[2];
//	vector<double> tempResults; 
//	double *tempResults; 
//	int iArraySize = data->size();
	int iArraySize = iSize;
	
	for (int i = 1;i < iArraySize ; i++ )
	{	
	//	double tempDifference = log(data->at(i)) - log(data->at(i-1)) ; 
		double tempDifference = log(data[i]) - log(data[i-1]) ; 
		dTotalGrowthRate += tempDifference;
	}
	return dTotalGrowthRate;
}

//double* Calibrate::paramCalculation(vector<double> *data)
double* Calibrate::paramCalculation(double *data, int iSize)
{	

	double dTotalGrowthRate = 0;
	double dTotalSigma=0;
	double *partialParameters = new double[2];
//	int iArraySize = data->size();
	int iArraySize = iSize;
//	vector<double> tempResults; 
	double *tempResults = new double[iSize]; 
	
	for (int i = 1; i < iArraySize; i++ )
	{	
		if (data[i-1] == 0)
			continue;
		double tempDifference =log(data[i]) - log(data[i-1]) ; 
	//	tempResults.push_back(tempDifference);
		tempResults[i - 1] = tempDifference;
		dTotalGrowthRate += tempDifference;
	}
	
	double average = dTotalGrowthRate / (iArraySize - 1);
	partialParameters[0] = average;
		
	for(int i = 1; i < iArraySize ; i++ )
	{
		// dTotalSigma += pow((tempResults.at(i-1) - average),2) ;	
		 dTotalSigma += pow((tempResults[i-1] - average),2) ;	
	}
	
	partialParameters[1] = sqrt(dTotalSigma / (iArraySize - 1));
	delete []tempResults;
	
	return partialParameters ;
}

// Calibrate the mu and varience in a multiple machines machine using single database
double* Calibrate::calcParamFromCentralDB()
{	
	return 0;	
}
