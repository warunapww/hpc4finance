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

using namespace boost;

class CGBMotion
{
private:
	variate_generator<mt19937&, normal_distribution<double> > *d_vGen;

	double m_nCurrValue; // initial security value (constant)
	double m_nDrift; // our drift (constant)
	double m_nSigma; // our volatility (constant)
	double m_nDeltaT; // the current elapsed time
	
public:

	CGBMotion(double nSInitial, double nDrift, double nSigma, double deltaT, double dSeed );
	~CGBMotion(void);
	double step();

};
