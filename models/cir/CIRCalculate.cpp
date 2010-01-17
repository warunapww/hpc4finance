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
#include "CIRCalculate.h"
#ifdef _WIN32
#include <winsock.h>
#endif // Win32
#include <boost/random.hpp>
#include <stdio.h>
#include <iostream>
#include "Vasicek.h"
#include "Calib.h"
#include <ctime>
#include <math.h>
#include <sys/timeb.h>
#include <vector>
//#include <mysql.h>
#include "DataExtracter.h"

CIRCalculate::CIRCalculate() {
	// TODO Auto-generated constructor stub

}

double* CIRCalculate::CalcCIRParralel(int iRank, int iSize, vector<double> *vData, double dTimeStep, int n, int iNoOfItrPerMachine, double lastData ){

	MPI_Status mpiStatus;

	Calib cal(vData);
	int iTag =1;
	double sx, sy, sxx, syy, sxy;
	double mu, lambda, sigma_square, sigma;
	double partialParameters[5];
	double finalParameters[3];

	partialParameters[0] = cal.getSx();
	partialParameters[1] = cal.getSy();//cout<<sy<<endl;;
	partialParameters[2] = cal.getSxx();//cout<<sxx<<endl;;
	partialParameters[3] = cal.getSyy();//cout<<syy<<endl;;
	partialParameters[4] = cal.getSxy();//cout<<sxy<<endl;;

	if(iRank != 0)
	{
		MPI_Send(&partialParameters, 5, MPI_DOUBLE, 0,iTag, MPI_COMM_WORLD);
	}
	else
	{
		vector<vector<double> > parametersCollection(iSize, vector<double> (5));
		double tempResults[5];

		for(int i=0; i< 5 ; i++ )
		{
			parametersCollection[0][i] = partialParameters[i];
		}

		for(int i=1; i< iSize; i++)
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
		
			for(int k=0; k < iSize ; k++)
			{
				temp = temp + parametersCollection[k][i];
			}

			if(i == 0)
				sx = temp;
			else if(i == 1)
				sy = temp;
			else if(i == 2)
				sxx = temp;
			else if(i == 3)
				syy = temp;
			else if(i == 4)
				sxy = temp;
		}
		mu = ((sy * sxx) - (sx * sxy)) / (21 * (sxx - sxy) - (pow(sx,2) - (sx * sy)));
		lambda = (-1/dTimeStep)*(log((sxy - mu*sx - mu*sy + n*pow(mu,2))/(sxx - 2*mu*sx + n*pow(mu,2))));
		double alpha = exp(-1*lambda*dTimeStep);
		double sigmahat_square = (syy - (2*alpha*sxy) + (pow(alpha,2)*sxx) - (2*mu*(1-alpha)*(sy - 				alpha*sx)) + (n*pow(mu,2)*(pow((1-alpha),2))))/n;
		sigma_square = sigmahat_square*2*lambda / (1 - pow(alpha,2));
	
		finalParameters[0] = mu;
		finalParameters[1] = lambda;
		finalParameters[2] = sqrt(sigma_square) ;
	}

	if (iRank==0)
	{
		MPI_Bcast ( &finalParameters, 3, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	}
	else
	{
		MPI_Bcast ( &finalParameters, 3, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	}

	mu = finalParameters[0];
	lambda =  finalParameters[1];
	sigma = finalParameters[2];
	
	double *CIRValues;

	CIRValues=  MonticarloSimulation(iRank, iSize, n, iNoOfItrPerMachine, mu, sigma, lambda, lastData );
	
	return CIRValues;
}

CIRCalculate::~CIRCalculate() {
	// TODO Auto-generated destructor stub
}

double* CIRCalculate::CalcCIRSequentially(int iRank, int iSize, vector<double> *vData,double dTimeStep, int n, int iNoOfItrPerMachine, double lastData)
{
	int iTag =1;
	MPI_Status mpiStatus;
	double finalParameters[3];
	double mu, lambda ,sigma;  
	
	if(iRank==0)
	{

		Calib cal(vData);

		double sx = cal.getSx();//cout<<sx<<endl;;
		double sy = cal.getSy();//cout<<sy<<endl;;
		double sxx = cal.getSxx();//cout<<sxx<<endl;;
		double syy = cal.getSyy();//cout<<syy<<endl;;
		double sxy = cal.getSxy();//cout<<sxy<<endl;;

		double mu = ((sy * sxx) - (sx * sxy)) / (21 * (sxx - sxy) - (pow(sx,2) - (sx * sy)));
		double lambda = (-1/dTimeStep)*(log((sxy - mu*sx - mu*sy + n*pow(mu,2))/(sxx - 2*mu*sx + n*pow(mu,2))));
		double alpha = exp(-1*lambda*dTimeStep);
		double sigmahat_square = (syy - (2*alpha*sxy) + (pow(alpha,2)*sxx) - (2*mu*(1-alpha)*(sy - alpha*sx)) + (n*pow(mu,2)*(pow((1-alpha),2))))/n;
		double sigma_square = sigmahat_square*2*lambda / (1 - pow(alpha,2));

		finalParameters[0] = mu;
		finalParameters[1] = lambda;
		finalParameters[2] = sqrt(sigma_square) ;
	}
	
	if (iRank==0)
	{
		MPI_Bcast ( &finalParameters, 3, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	}
	else
	{
		MPI_Bcast ( &finalParameters, 3, MPI_DOUBLE , 0, MPI_COMM_WORLD );
	}	

	mu = finalParameters[0];
	lambda =  finalParameters[1];
	sigma = finalParameters[2];
	//CIR(double dSInitial, double dDrift, double dSigma, double dMeanReversionSpeed, double dDelta)
	
	double *CIRValues;

	CIRValues=  MonticarloSimulation(iRank, iSize, n, iNoOfItrPerMachine, mu, sigma, lambda, lastData );
	
	return CIRValues;

}

double* CIRCalculate::MonticarloSimulation(int iRank, int iSize, int n, int iNoOfItrPerMachine, double mu, double sigma, double lambda, double lastData )
{
	MPI_Status mpiStatus;
	int iTag = 1;
	double dTimeStep = (1.0/12.0);

	double CIRValues[n+1];
	vector<vector<double> > results(n+1, vector<double> (iNoOfItrPerMachine));

	for (int i=0; i<iNoOfItrPerMachine; i++)
	{
		/*Vasicek vasicek_test(dInitial, dDrift, dSigma, dLambda, dTimeStep);*/
		Vasicek vasicek_test(lastData, mu, sigma, lambda, dTimeStep);
		for (int j=0; j < n+1 ;j++)
		{
			results[j][i] = vasicek_test.step();
		}
	}

	double caliberatedValues[n+1];
	
	// calibration on simulated data
	unsigned int x,y;

	for(x = 0;x < results.size();x++)
	{
		double sum = 0;
		for(y = 0;y < results[x].size();y++)
		{
			sum += results[x][y];
		}
		caliberatedValues[x] = sum / results[x].size();
	}

	if(iRank != 0)
	{
		MPI_Send(&caliberatedValues, n+1, MPI_DOUBLE, 0,iTag, MPI_COMM_WORLD);
	}
	else
	{
		vector<vector<double> > finalResults(iSize, vector<double> (n+1));
		double tempResults[n+1];

		for(int i=0; i< n+1 ; i++ )
		{
			finalResults[0][i] = caliberatedValues[i];
		}

		for(int i=1; i< iSize; i++)
		{
			MPI_Recv(&tempResults, n+1, MPI_DOUBLE, i, iTag, MPI_COMM_WORLD, &mpiStatus);

			for(int k = 0; k < n+1; k++)
			{
				finalResults[i][k] = tempResults[k];
			}
		}

		for (int i=0; i < n+1 ; i++)
		{
			double temp = 0;
			for(int k =0; k < iSize ; k++)
			{
				temp  = temp + finalResults[k][i];
			}
			CIRValues[i] = temp/iSize;

			printf("%f", CIRValues[i]);
			fflush(stdout);
			
			if(i != n)
			{
				printf(",");
				fflush(stdout);
			}
		}
		
		printf("\n");
		fflush(stdout);
	}
	
	return CIRValues;
}
