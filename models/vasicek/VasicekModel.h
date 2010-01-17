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
#include <math.h>
#include <boost/random.hpp>
#include <vector>

using namespace boost;
//typedef variate_generator<mt19937&, normal_distribution<>> RANDOMNUMBER

class VasicekModel
{
private:
	std::vector<double> *v11;
	double d_SInitial; // initial security value (constant)
	double d_Drift; // our drift
	double d_Sigma; // our volatility
	double d_Lambda; // our mean reversion rate
	double d_Delta; // our time step
	double d_Diff; // how much the process has diffused
	double d_CurrValue; // value at the end of current time step

	variate_generator<mt19937&, normal_distribution<double> > *d_vGen;
	//RANDOMNUMBER *d_vGen;
	
public:
	VasicekModel(double dSInitial, double dDrift, double dSigma, double dMeanReversionSpeed, double dDelta, double dSeed);
	double step(void);
	~VasicekModel(void);
	double getSx();
	double getSy();
	double getSxx();
	double getSyy();
	double getSxy();
};
