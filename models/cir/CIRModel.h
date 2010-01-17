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

#ifndef CIR_H_
#define CIR_H_
#pragma once
#include <boost/random.hpp>
#include <vector>
#include "newran.h"

using namespace boost;

typedef variate_generator<mt19937, poisson_distribution<> > rnd_poisson_t;
typedef variate_generator<mt19937, normal_distribution<double> > rnd_normal_t;

class CIRModel {

private:
	rnd_poisson_t *d_vGen1;
	rnd_normal_t *d_vGen2;
	
	MT *urng;
	
	double d_CurrVal; // initial security value (constant)
	double d_Drift; // our drift
	double d_Sigma; // our volatility
	double d_Alpha; // our mean reversion rate
	double d_TimeStep; // our time step
	double d_CurrentTime; // the current elapsed time
	double d_d;

	void invokePoisson(double dLambda);
	
public:
	CIRModel(double dSInitial, double dDrift, double dSigma, double dMeanReversionSpeed, double dTimeStep, double dSeed);
	double step(void);
	virtual ~CIRModel();
};

#endif /* CIR_H_ */
