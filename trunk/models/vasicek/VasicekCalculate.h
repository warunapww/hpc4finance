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
#include <mpi.h>
#include <vector.h>


class VasicekCalculate {


public:
	VasicekCalculate();
	virtual ~VasicekCalculate();

public:
	double* CalcVasicekParralel(int iRank, int iSize, vector<double> *vData, double dTimeStep, int n, int iNoOfItrPerMachine, double lastData );
	double* CalcVasicekSequentially(int iRank, int iSize, vector<double> *vData,double dTimeStep, int n, int iNoOfItrPerMachine, double lastData);
	double* MonticarloSimulation(int iRank, int iSize, int n, int iNoOfItrPerMachine, double mu, double sigma, double lambda, double lastData );
};

//#endif /* VASICEKCALCULATE_H_ */
