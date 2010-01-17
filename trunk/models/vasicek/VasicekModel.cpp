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

#include "VasicekModel.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

using namespace boost;


VasicekModel::VasicekModel(double dSInitial, double dDrift, double dSigma, double dMeanReversionSpeed, double dTimeStep, double dSeed)
{
	static mt19937 engine(static_cast<unsigned> (dSeed));
	normal_distribution<> normal_dist;
	d_vGen = new variate_generator<mt19937&, normal_distribution<double> >(engine, normal_dist);
	v11 = new vector<double>;
	/*cout << (*d_vGen)() << endl;
	cout << (*d_vGen)() << endl;*/
	d_SInitial = dSInitial;
	d_Drift = dDrift;
	d_Sigma = dSigma;
	d_Lambda = dMeanReversionSpeed;
	d_Delta = dTimeStep;
	d_CurrValue = dSInitial;
	d_Diff = 0;

}

VasicekModel::~VasicekModel(void)
{
	delete d_vGen;
}

double VasicekModel::step(void)
{
	//cout << (*d_vGen)() << endl;
	
	double dRandomNumber = (*d_vGen)(); 
		double d_Diff = d_Drift * (1 - exp(-1 * d_Lambda * d_Delta)) + d_Sigma * sqrt((1 - exp(-2 * d_Lambda * 								d_Delta)) / (2 * d_Lambda)) * dRandomNumber;
		d_CurrValue = d_CurrValue*exp(-d_Lambda*d_Delta) + d_Diff;

	return d_CurrValue;

}
