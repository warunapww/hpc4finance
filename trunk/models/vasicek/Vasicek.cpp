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

// Vasicek.cpp : Defines the entry point for the console application.
//
#include <mpi.h>
#include <stdio.h>
#include "VasicekModel.h"
#include "math.h"
#include <stdlib.h>
#include <ctype.h>
#include <vector>
#include "Calibrate.h"
#include "DataExtracter.h"
#include "def.h"
#include <string.h>
#include "Vasicek.h"
#include "Logs.h"
#include "duration.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////
/*
Euler Discretization of Vasicek is given at http://www.quantcode.com/modules/mydownloads/singlefile.php?cid=9&lid=390
However it has been shown that Exact Discretization is more accurate than Euler's
Therefore exact discretization equation given at http://www.quantcode.com/modules/mydownloads/singlefile.php?cid=9&lid=391
has been used
*/
//////////////////////////////////////////////////////////////////////////
#define WRITE_LOG

extern int i_Rank;


Vasicek::Vasicek(int argCount, char **args, char* pzDBName, char* pzIP, char* pzUserName, char* pzPW, char* pzDataColumn)
{
	i_ArgCount = argCount;
	pz_Args = args;
	pz_DBName = strdup(pzDBName);
	pz_UserName = strdup(pzUserName);
	pz_Password = strdup(pzPW);
	pz_IPAddress = strdup(pzIP);
	pz_DataColumn = strdup(pzDataColumn);
	pz_TableName = NULL;
  	
	int iRc = MPI_Init(&i_ArgCount, &pz_Args);
	if (iRc != MPI_SUCCESS)
	{
		 printf ("error,Error starting MPI program. Terminating.\n");
		 MPI_Abort(MPI_COMM_WORLD, iRc);
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &i_Rank);
	MPI_Comm_size(MPI_COMM_WORLD, &i_Size);
	
	#ifdef WRITE_LOG
	poTansLog = new Logs(LOGTYPE);
	poTansLog->WriteLog(HEADLINE);
	
	Duration d;
	poTansLog->WriteLog(d.getSysTime());
	#endif 
}

double* Vasicek::getParams()
{
	double dInitial, dDrift, dSigma, dLambda;
	
//	bool bValidParams = validateParams(i_ArgCount, pz_Args);
	bool bValidParams = true;
	
	if(!bValidParams)
	{
		if (i_Rank == 0)
		{
			printf("error,Incorrect parameters\n");
			fflush(stdout);
		}
		//MPI_Finalize();
		return (double*) 0;
	}
	int iStdErr = dup(fileno(stderr));
	close(fileno(stderr));
	int iOption;
	
  	if(i_ArgCount == 9)
  	{
  		dInitial = (double)atof(pz_Args[1]);
		dDrift = (double)atof(pz_Args[2]);
		dSigma = (double)atof(pz_Args[3]);
		dLambda = (double)atof(pz_Args[4]);
				
  		while ( (iOption = getopt(i_ArgCount, pz_Args, "d:m:q:y:s:")) != -1 )
        	{
		        switch (iOption)
		        {
		        case 'd':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 252.0;
		                //dDrift /= 252;
		               // dSigma /= sqrt(252);
		                break;
		        case 'm':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 12.0;
		                //dDrift /= 12;
		               // dSigma /= sqrt(12);
		                break;
		        case 'q':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 4.0;
		                //dDrift /= 4;
		                //dSigma /= sqrt(4);
		                break;
		        case 'y':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0;
		                break;
		        case 's':
		                i_Iterations = (int)atoi(optarg);
		                break;
		        default: 
		        	if(i_Rank == 0)
		  		printf("error,Incorrect parameters from wrong arguments\n");
		  		fflush(stdout);
		  		//MPI_Finalize();
			  	return 0;
		        }
        	}
        	if (i_Forecasts == 0)
		  {
		  	if(i_Rank == 0)
		  	{
				printf("error,Incorrect parameters\n");
				fflush(stdout);
			}
			//MPI_Finalize();
	  		return 0;
		  }
        	//d_TimeStep = 1.0 / (double)i_Forecasts;
        	//printf ("switches: %d:%f\n", i_Forecasts, d_TimeStep);
  	}
  	else if(i_ArgCount == 7)
  	{	
  		double factorDrift, factorSigma;
  		while ( (iOption = getopt(i_ArgCount, pz_Args, "t:d:m:q:y:s:")) != -1 )
        	{
		        switch (iOption)
		        {
		        case 't':
		        	pz_TableName = optarg;
		        	break;
		        case 'd':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 252.0;
		                //factorDrift = 252.0;
		                factorSigma = sqrt(252.0);
		                break;
		        case 'm':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 12.0;
		                //factorDrift = 252 / 12;
				factorSigma = sqrt(12);
		                break;
		        case 'q':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0 / 4.0;
		               // factorDrift = 252 / 4;
		                factorSigma = sqrt(4);
		                break;
		        case 'y':
		                i_Forecasts = (int)atoi(optarg);
		                d_TimeStep = 1.0;
		                //factorDrift = 252.0;
		                factorSigma = 1.0;
		                break;
		        case 's':
		                i_Iterations = (int)atoi(optarg);
		                break;
		                
		        default: 
		        	if(i_Rank == 0)
		  		printf("error,Incorrect parameters from wrong arguments\n");
		  		fflush(stdout);
		  		//MPI_Finalize();
			  	return 0;
		        }
        	}
        	if (i_Forecasts == 0)
		  {
		  	if(i_Rank == 0)
		  	{
				printf("error,Incorrect parameters\n");
				fflush(stdout);
			}
			//MPI_Finalize();
	  		return 0;
		  }
        	dup2(iStdErr, fileno(stderr));
        	
        	//write(fileno(stderr), "fffff", 5); checking if stdErr is working

  		Calibrate *poCalc = new Calibrate(pz_DBName, pz_IPAddress, pz_UserName, pz_Password, pz_TableName, pz_DataColumn, 														i_Rank, i_Size, 1.0/252.0);
  		bool bTableStatus = poCalc->checkTable();
  		
  		if (!bTableStatus)
  		{
			if (i_Rank == 0)
			{
				printf("error,No records found in Database");
				fflush(stdout);
			}
  			return 0;
  		}
  		
  		double *pdCalibValues;
  		pdCalibValues = poCalc->calcParameters();
  		
  		dInitial = pdCalibValues[0] ;
		dDrift = pdCalibValues[1] ;
		dSigma = pdCalibValues[2];
		dLambda = pdCalibValues[3];
	  }
	  else 
	  {
	       	if(i_Rank == 0)
	  		printf("error,Incorrect parameters1\t%d\n", i_ArgCount);
	  	
	  	//MPI_Finalize();
	  	return 0;
	  }
	  
	#ifdef WRITE_LOG

	char pzJob[128];
	sprintf(pzJob, "MC trials \t%d \nPredictions \t%d \n",(int)i_Iterations/i_Size, i_Forecasts);
	poTansLog->WriteLog(pzJob);
		
	char pzCalibValues[256];
	sprintf(pzCalibValues, "Initial Value \t%f \nMean \t\t%f \nSigma \t\t%f \nMRS \t\t%f\nTime step\t%f \n",      									dInitial, dDrift, dSigma, dLambda, d_TimeStep);
	poTansLog->WriteLog(pzCalibValues);
	
	#endif 
	//printf("params: %f\t%f\t%f\t%f\t%f\n",dInitial, dDrift, dSigma, dLambda, d_TimeStep);
	//fflush(stdout);
	  double* pd_Params = new double[4];
	  pd_Params[0] = dInitial;
	  pd_Params[1] = dDrift;
	  pd_Params[2] = dSigma;
	  pd_Params[3] = dLambda;

	  return pd_Params;
}

int Vasicek::getNthPrime(int n)
{
	int count = 0;
	int primeNo = 0;
	int k = 2;
	while (count < n)
	{
		
		if (k == 2)
		{         // the only even prime
			primeNo = 2;
			count++;	
		}
		else if (k % 2 == 0)     // other even numbers are composite
		{
			k++;
			continue;
			
		}
		else
		{
			bool prime = true;
			int divisor = 3;
			int upperLimit = static_cast<int>(sqrt(k) + 1);
			while (divisor <= upperLimit)
			{
				
				if (k % divisor == 0)
       	 			prime = false;
    				divisor +=2;
			}
			if (prime == true)
			{
				primeNo = k;
				count++;
			}
		}
		k++;
	}
	return primeNo;
}

double* Vasicek::getPredicts(double *dParams)
{
	MPI_Status mpiStatus;
	int iTag = 1;
	
	char psIPAddress[128];
	gethostname(psIPAddress, sizeof psIPAddress);
	
	int iSimsPerNode = i_Iterations / i_Size;
	double dPerformaceRatio = 0;
	double dPerformTotalRatio = 0;
	int iGlobFreq = 0;
	
	
	//check for file existance
	Logs *poRatios = new Logs();
	bool isFileExists = poRatios->checkFileExists("tuning.txt") ;
	
	//tokenized tuning details
	vector<vector<string> > *tuneFileList;
	int iMachines = 0;
	
	//////////////////////////////////////////////////////////
	if(isFileExists)
	{
		char *cBuffer = poRatios->readLog("tuning.txt");
		vector<string> *PerNodeRatio =  poRatios->tokenizer(cBuffer, ';');
	
		iMachines = PerNodeRatio->size();
		tuneFileList = new vector<vector<string> >(iMachines, vector<string> (3));

		// read the tuning file
		for(int i=0 ; i < iMachines; i++)
		{
			string  temp = PerNodeRatio->at(i);
			vector<string> *tmp= poRatios->tokenizer(temp, ' ');
	
			for(int j = 0; j < 3  ; j++)
			{
				if(j != 2)
					(*tuneFileList)[i][j] = tmp->at(j);
				else
					(*tuneFileList)[i][j] = NOTVISITED;
			}			
		}
		
		if(i_Rank != 0)
			MPI_Send(&psIPAddress, 128, MPI_CHAR, 0, iTag, MPI_COMM_WORLD);
		
		else
		{	
			int iNewMachinFreq = 0; 					// count new machines
			char tempIPAddress[128];
			vector< char* > *pIPRankDatails = new vector< char* >(i_Size);	// IP list according to their rank  
			vector< string *> *pvNewMachines = new vector< string *>;	// IP List of new machines
			
			(*pIPRankDatails)[0] = strdup(psIPAddress);
		
			for(int i =1; i < i_Size; i++)
			{
				MPI_Recv(&tempIPAddress, 128, MPI_CHAR, i, iTag, MPI_COMM_WORLD, &mpiStatus);
				(*pIPRankDatails)[i] = strdup(tempIPAddress);
			}			
			for(int j = 0; j < i_Size ; j++)
			{
				bool isMachineExists = false;
				string sTempIPRank (pIPRankDatails->at(j));

				// check whether the machine exists
				for (int i =0; i < iMachines ; i++ )
				{
					string sTempTuneList((*tuneFileList)[i][0]); 
					if( sTempTuneList.compare(sTempIPRank) == 0)
					{
						isMachineExists = true;
						break;
					}
				}
									
				if( !isMachineExists )
				{		
					bool bInMachineList = false;
					for (int k =0; k < pvNewMachines->size() ; k++ )
					{ 
						if( sTempIPRank.compare((*pvNewMachines)[k][0]) == 0)
						{
							int iTempFreq = atoi((*pvNewMachines)[k][1].c_str());
							iTempFreq++;
							char sTempFreq[sizeof(int) + 1];
							sprintf( sTempFreq,"%d",iTempFreq);
							(*pvNewMachines)[k][1] = sTempFreq;			
							bInMachineList = true;
						}
					}
					
					if(!bInMachineList)
					{
						string *vTemp = new string[2];
						vTemp[0] = pIPRankDatails->at(j);
						char sTempFreq[sizeof(int) + 1];
						sprintf( sTempFreq,"%d",1);
						vTemp[1] = sTempFreq;
						pvNewMachines->push_back(vTemp);
						iNewMachinFreq++;
						iGlobFreq++;
					}
				}
			}

			// if new machine are not added			
			vector<vector<string> *> *frequencyList = new vector<vector<string> *>;
			double tempTotal = 0;
			double dtempRatios = 0;
			
			// keep machine name : total ratio
			vector< string > *pvFreqListElement;

			for (int i = 0; i < iMachines; i++)
			{
	
				pvFreqListElement = new vector< string >(3);
				int frequency=0;

				for (int j = 0; j < i_Size; j++)
				{
					if((*tuneFileList)[i][0].compare(pIPRankDatails->at(j))==0)
						frequency++;
				}

				// if machine is used for run the process insert to the frequency list
				if(frequency > 0)
				{
					tempTotal += frequency * (atof((*tuneFileList)[i][1].c_str()));
					dPerformTotalRatio += atof((*tuneFileList)[i][1].c_str());
					
					char tempDRatio[sizeof(double) +1];
					sprintf( tempDRatio,"%f",(frequency * (atof((*tuneFileList)[i][1].c_str()))));
					(*pvFreqListElement)[0] = (*tuneFileList)[i][0];
					(*pvFreqListElement)[1] = tempDRatio;
					(*pvFreqListElement)[2] = (*tuneFileList)[i][1];
					frequencyList->push_back(pvFreqListElement);
					iGlobFreq++;
				}
			}

			for(int i = 0; i < i_Size ; i++ )
			{
				bool bNewFile = true;
				for(int j = 0; j < frequencyList->size(); j++)
				{
					if(string((*pIPRankDatails)[i]).compare((*(*frequencyList)[j])[0]) == 0)
					{
						double tempVal = atof((*(*frequencyList)[j])[2].c_str()) / tempTotal ;		
						dtempRatios = tempVal * ( (double)(i_Size- iNewMachinFreq)/(double)i_Size);
						
						if(i != 0)
							MPI_Send(&dtempRatios, 1, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD);
						else
							dPerformaceRatio = dtempRatios;
							
						bNewFile = false;	
						break;
					}
				}
				if(bNewFile)
				{
					dtempRatios = 1.0 / (double)i_Size;
					if(i != 0)
						MPI_Send(&dtempRatios, 1, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD);
					else
						dPerformaceRatio = dtempRatios;	
				}
				
			}
				
			delete frequencyList;		
			delete pIPRankDatails;
		}

		if(i_Rank != 0)
			MPI_Recv(&dPerformaceRatio, 1, MPI_DOUBLE, 0, iTag, MPI_COMM_WORLD, &mpiStatus);
		
		iSimsPerNode = (int)(dPerformaceRatio * i_Iterations);

	}
	else
		iGlobFreq = i_Size;
	
    	
    	    	  	
    	double dInitial = dParams[0];
    	double dDrift = dParams[1];
    	double dSigma = dParams[2];
    	double dLambda = dParams[3];
    	
    	Duration oDuration;
	oDuration.setStart();
    	
    	double dCurrTime = 0;
    	
    	if(i_Rank == 0)
    	{
    		dCurrTime = (double)oDuration.getCurrTime();	
    	}
    	
	MPI_Bcast ( &dCurrTime, 1, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	
	double dSeed = dCurrTime * getNthPrime( i_Rank + i_Size);
	
	char cSeed[32];
	sprintf(cSeed , "Seed value :%f \n" , dCurrTime);
	#ifdef WRITE_LOG
	poTansLog->WriteLog(cSeed);	
	#endif
				
	vector<vector<double> > *pvResults = new vector<vector<double> >((i_Forecasts), vector<double> (iSimsPerNode)); 
		
	for (int i = 0; i < iSimsPerNode ; i++)
	{	
		VasicekModel oVasicek(dInitial, dDrift, dSigma, dLambda, d_TimeStep, dSeed);  // our brownian motion object 

		for (int k = 0; k < i_Forecasts; k++)
		{
			(*pvResults)[k][i] = oVasicek.step();
		}
	}
	double daLocalCalibVals[i_Forecasts];
	
	for(int i=0; i< (i_Forecasts) ; i++ )
	{
		double dTotal = 0;	
				
		for(int k= 0; k < iSimsPerNode; k++ )
		{
			dTotal += (*pvResults)[i][k];
		}
				
		//daLocalCalibVals[i]= dTotal / iSimsPerNode;
		daLocalCalibVals[i]= dTotal ;
	}

	delete pvResults;
	
oDuration.setEnd();
	double dDuration = oDuration.getPassedTime() / iSimsPerNode;
	double dInvDuration = 1/dDuration;
		
	if (i_Rank != 0)
	{	
		MPI_Send(&daLocalCalibVals, i_Forecasts, MPI_DOUBLE, 0,iTag, MPI_COMM_WORLD);	
		MPI_Send(&dInvDuration, 1, MPI_DOUBLE, 0,iTag, MPI_COMM_WORLD);
		MPI_Send(&psIPAddress, 128, MPI_CHAR, 0,iTag, MPI_COMM_WORLD);			
	}
	
	double *dPredictValues = new double[i_Forecasts];
	double *dInvDurationValues= new double[iGlobFreq];	
	char **pzIPAddress = new char*[iGlobFreq];
	for (int i = 0; i < iGlobFreq; i++)
		pzIPAddress[i] = new char[128];

	if (i_Rank == 0) 
	{		
		double dTempDuration = 0;			
		double dTotal = 0;
				
		dInvDurationValues[0] = dInvDuration;
		dTotal = dInvDurationValues[0];
		strcpy(pzIPAddress[0], psIPAddress);

		double dTempResults[i_Forecasts];			
		vector<vector<double> > finalResults(i_Size, vector<double> (i_Forecasts));
				
		for (int i=0; i < i_Forecasts; i++)
			finalResults[0][i] = daLocalCalibVals[i] ;
				
		// Wait for results from other processes 
		for (int i = 1; i < i_Size; i++) 
		{		
				
			MPI_Recv(&dTempResults, i_Forecasts, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD, &mpiStatus);
					
			for (int k=0; k < i_Forecasts; k++)
				finalResults[i][k] = dTempResults[k];
		
			char tempIP[128];
			MPI_Recv(&dTempDuration, 1, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD, &mpiStatus);
			MPI_Recv(&tempIP, 128, MPI_CHAR, i, iTag, MPI_COMM_WORLD, &mpiStatus);
			
			bool bReplicatedMachine = false;
			
			for(int j = 0;j < i ;j++)
			{
				if(string(tempIP).compare(string(pzIPAddress[j])) == 0)
				{
					double dTemp = 	dInvDurationValues[j];
					dInvDurationValues[j] = (dInvDurationValues[j]+dTempDuration) / 2;
					bReplicatedMachine = true;
					dTotal = dTotal	- dTemp + dInvDurationValues[j];
				}
			}
			
			if(!bReplicatedMachine)
			{	
				strcpy(pzIPAddress[i], tempIP);
				dInvDurationValues[i] = dTempDuration;
				dTotal += dInvDurationValues[i];
			}
		}

		for (int i = 0; i < i_Forecasts; i++ )
		{
			double totalForcastValues = 0;
				
			for( int k =0; k < i_Size ; k++ )
				totalForcastValues  += finalResults[k][i];
		
			dPredictValues[i] = totalForcastValues/ i_Iterations;
		
			
			if(i != i_Forecasts-1)
			{
		
			}
		}
		
		/*for(int i=0; i < iGlobFreq; i++)
		{
			printf("%s:%f \n",pzIPAddress[i],dInvDurationValues[i]);
		}*/
		
		double *dRatio = new double[iGlobFreq];
		char **pzTuningIPAddress ;
		double *dModifiedRatio; 
		
		if(i_Size < iMachines )
		{
			dModifiedRatio = new double[iMachines];
			pzTuningIPAddress = new char*[iMachines];
			for (int i = 0; i < iMachines; i++)
				pzTuningIPAddress[i] = new char[128];
		}
		else
		{
			dModifiedRatio = new double[i_Size];
			pzTuningIPAddress = new char*[i_Size];
			for (int i = 0; i < i_Size; i++)
				pzTuningIPAddress[i] = new char[128];
		}
	
		for(int i = 0; i < iGlobFreq ;i++)
		{
			dRatio[i] = dInvDurationValues[i] / dTotal;
		}
	
		Logs *poTune = new Logs();
		
		if((i_Size == iMachines) ||(iMachines == 0 && !isFileExists))
		{
			if(isFileExists)
			{					
				for(int j = 0; j < iGlobFreq ; j++)
				{
					for (int i =0; i < iMachines ; i++ )
					{
						if( (*tuneFileList)[i][0].compare(pzIPAddress[j])==0)
						{
							dModifiedRatio[j] = (dRatio[i] + atof((*tuneFileList)[i][1].c_str()))/2;
							break;
						}
					}	
				}
				if(i_Size> 2)
					poTune->writeTuning( dModifiedRatio, pzIPAddress , iGlobFreq);
			}
			else
			{
				if(i_Size > 2)
					poTune->writeTuning( dRatio, pzIPAddress , iGlobFreq);
			}
		}
		else if(i_Size != iMachines )
		{
			if(isFileExists)
			{					
				for(int j = 0; j < iGlobFreq ; j++)
				{
					bool bNewMachine = true;
					for (int i =0; i < iMachines ; i++ )
					{
						if( (*tuneFileList)[i][0].compare(pzIPAddress[j])==0)
						{
							dModifiedRatio[j] = (( dRatio[i] * dPerformTotalRatio)+ 			 	     								atof((*tuneFileList)[i][1].c_str()))/2;
							strcpy(pzTuningIPAddress[j], pzIPAddress[j]);
							(*tuneFileList)[i][2] = VISITED; 
							bNewMachine = false;
							break;
						}
					}
					if(bNewMachine)
					{
						dModifiedRatio[j] = dRatio[j]/2.0;
						strcpy(pzTuningIPAddress[j], pzIPAddress[j]);
					}	
				}
	
				int tempIncrement =0;
				for (int i =0; i < iMachines ; i++ )
				{
					if( (*tuneFileList)[i][2].compare(NOTVISITED) == 0)
					{	
						if(i_Size <= iMachines)
						{
							dModifiedRatio[tempIncrement+i_Size] = atof((*tuneFileList)[i][1].c_str());
							strcpy(pzTuningIPAddress[tempIncrement+i_Size], (*tuneFileList)[i][0].c_str());
							tempIncrement++;		
						}
						else
						{
							dModifiedRatio[tempIncrement+iMachines] = atof((*tuneFileList)[i][1].c_str());
							strcpy(pzTuningIPAddress[tempIncrement+iMachines], (*tuneFileList)[i][0].c_str());
							tempIncrement++;		
						}
					}
				}
				if(i_Size > 2)
				{	
					if( iMachines >= i_Size )
						poTune->writeTuning( dModifiedRatio, pzTuningIPAddress , iMachines);
					else
						poTune->writeTuning( dModifiedRatio, pzTuningIPAddress , iGlobFreq );
				}
			}
		}
	}
	
	delete [] dInvDurationValues;	
	for (int i = 0; i < iGlobFreq; i++)
	{
		delete []pzIPAddress[i];
	}
	delete [] pzIPAddress;
		
	MPI_Bcast (dPredictValues, i_Forecasts, MPI_DOUBLE, 0, MPI_COMM_WORLD);
 	return dPredictValues;
}

Vasicek::~Vasicek(void)
{
	if (pz_DBName)
		delete pz_DBName;
	if (pz_UserName)
		delete pz_UserName;
	if (pz_Password)
		delete pz_Password;
	//if (pz_TableName)
	//	delete pz_TableName;//TODO: FIND OUT WHY MEMORY CORRUPTION
	if (pz_IPAddress)
		delete pz_IPAddress;
	if (pz_DataColumn)
		delete pz_DataColumn;
		
	MPI_Finalize();
	#ifdef WRITE_LOG
	poTansLog->WriteLog("-END- \n");
	delete poTansLog;
	#endif
}

bool Vasicek::validateParams(int i_ArgCount,char* args[])
{
	if(i_ArgCount == 9)
  	{
  		for(int i = 1; i < i_ArgCount; i++)
		 {
		 	int iStrLen = strlen(args[i]);
		 	int iDots = 0;
 	
		 	for (int j = 0; j < iStrLen; j++)
		 	{
		 		if(isdigit(args[i][j]) == 0)
		 		{
		 			if (j == 0 && args[i][j] == '-')
		 			{
		 				continue;
		 			}
		 			
		 			if(args[i][j] != '.')
		 			{
		 				return false;
		 			}
		 			else
		 			{
		 				iDots++;
		 				
		 				if ((iStrLen == 1) || (iDots > 1))
		 				{
			 				return false;
		 				}
		 			}
		 		}
		 	}
		 }
		 return true;		 
  	}
  	else if(i_ArgCount == 7)
  	{	
  		if((strcmp(args[1], "-t") == 0)  && ((strcmp(args[3], "-d") == 0) || (strcmp(args[3], "-m") == 0)) && (strcmp(args[5], "-n") == 0))
  		{
  			return true;
  		}
 	}
	return false;	
}

/*bool validateParams(int argc,char* argv[]);// parameter validation

int main(int argc, char* argv[])
{
  
  int iRc = MPI_Init(&argc, &argv);
  if (iRc != MPI_SUCCESS)
  {
	  printf ("Error starting MPI program. Terminating.\n");
	  MPI_Abort(MPI_COMM_WORLD, iRc);
	  return 0;
  }

  	int iRank, iSize;

  	MPI_Comm_rank(MPI_COMM_WORLD, &iRank);
  	MPI_Comm_size(MPI_COMM_WORLD, &iSize);
  	MPI_Status mpiStatus;
	int iTag = 1;

	int iNumTrials = 50000; // number of MonteCarlo trials
	int const iPredicts = 60; // number of forward predictions //TODO: Paramerterize predictions
	int iNoOfItrPerMachine =  iNumTrials/iSize ;
	double dTimeStep = (1.0/12.0); // monthly
	
	bool bValidParams = validateParams(argc, argv);
	
	if(!bValidParams)
	{
		if (iRank == 0)
		{
			printf("Incorrect Parameters1\n");
			fflush(stdout);
		}
		MPI_Finalize();
		return 0;
	}
	
	double *pdParams;
	
	if(argc == 5)
	{
		pdParams = new double[4];
		pdParams[0] = (double)atof(argv[1]);
		pdParams[1] = (double)atof(argv[2]);
		pdParams[2] = (double)atof(argv[3]);
		pdParams[3] = (double)atof(argv[4]);		
	}
	else if(argc == 3)
	{
		int iError;
		if(iRank == 0)
		{
			char *pztableName = argv[2];
			Calib oCalib(pztableName, dTimeStep);
			//cout << "table name: " << pztableName << endl;
			pdParams = oCalib.Calibrate();
			if (pdParams == NULL)
			{
				MPI_Abort(MPI_COMM_WORLD, 0);// TODO: finalize mpi without abort
				//MPI_Finalize();
				return 0;
			}
		}
		else
		{
			pdParams = new double[4];
		}
		if (pdParams == NULL)
		{
			MPI_Finalize();
	  		return 0;
		}
		MPI_Bcast ( pdParams, 4, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	}
	
	// monte carlo simulations
	//---------------------------------------------------------------------------------------------------
	
	double daPredictValues[ iPredicts ];
	vector<vector<double> > vLocalResults(iPredicts , vector<double> ( iNoOfItrPerMachine ));

	cout << "Params: " << pdParams[0] << " : " << pdParams[1] << " : " << pdParams[2] << " : " << pdParams[3] << " : "<< dTimeStep << endl;
	for (int i = 0; i < iNoOfItrPerMachine; i++ )
	{
		//NOTE: Intitial value, mu, volatility etc should be in decimal values e.g: for 6 per cent interest rate -> use 0.06
		Vasicek vasicek_test(pdParams[0], pdParams[1], pdParams[2], pdParams[3], dTimeStep);
		int j;
		for (j=0; j < iPredicts; j++)
		{
			vLocalResults[j][i] = vasicek_test.step();
		}
		//printf("result: %f\n", vLocalResults[j-1][i]);
		//fflush(stdout);
	}
	delete []pdParams;
	double daAvgLocResults[iPredicts];

	for(unsigned int i = 0; i < vLocalResults.size() ; i++)
	{
		double dSum = 0;
		for(unsigned int j = 0; j < vLocalResults[i].size(); j++)
		{
			dSum += vLocalResults[i][j];
		}
		daAvgLocResults[i] = dSum / vLocalResults[i].size();
	}

	if(iRank != 0)
	{
		MPI_Send(&daAvgLocResults, iPredicts, MPI_DOUBLE, 0, iTag, MPI_COMM_WORLD);
	}
	else
	{
		vector<vector<double> > vaPatialResults(iSize, vector<double> (iPredicts));
		double daTempResults[iPredicts];
		
		for(int i = 0; i < iPredicts ; i++ )
		{
			vaPatialResults[0][i] = daAvgLocResults[i];
		}

		for(int i = 1; i < iSize; i++)
		{
			MPI_Recv(&daTempResults, iPredicts, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD, &mpiStatus);

			for(int k = 0; k < iPredicts; k++)
			{
				vaPatialResults[i][k] = daTempResults[k];
			}
		}

		for (int i = 0; i < iPredicts ; i++)
		{
			double dTemp = 0;
			for(int k = 0; k < iSize ; k++)
			{
				dTemp += vaPatialResults[k][i];
			}
			daPredictValues[i] = dTemp / iSize;

			printf("%f", daPredictValues[i]);
			fflush(stdout);
			
			if(i != iPredicts - 1)
			{
				printf("\n");
				fflush(stdout);
			}
		}
		printf("\n");
		fflush(stdout);
	}
	MPI_Finalize();

	return 0;
}

bool validateParams(int argc,char* argv[])
{
	if(argc == 5)
  	{
  		for(int i = 1; i < argc; i++)
		 {
		 	int iStrLen = strlen(argv[i]);
		 	int iDots = 0;
 	
		 	for (int j = 0; j < iStrLen; j++)
		 	{
		 		if(isdigit(argv[i][j]) == 0)
		 		{
		 			if (j == 0 && argv[i][j] == '-')
		 			{
		 				continue;
		 			}
		 			
		 			if(argv[i][j] != '.')
		 			{
		 				return false;
		 			}
		 			else
		 			{
		 				iDots++;
		 				
		 				if ((iStrLen == 1) || (iDots > 1))
		 				{
			 				return false;
		 				}
		 			}
		 		}
		 	}
		 }
		 return true;		 
  	}
  	else if(argc == 3)
  	{	
  		if(strcmp(argv[1], "-t") == 0)
  		{
  			return true;
  		}
 	}
	return false;	
}*/
