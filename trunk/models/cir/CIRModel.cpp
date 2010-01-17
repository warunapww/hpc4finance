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

#include "CIRModel.h"
#include <iostream>
#include "duration.h"
#include "newran.h"


using namespace boost;
using namespace std;

CIRModel::CIRModel(double dSInitial, double dDrift, double dSigma, double dMeanReversionSpeed, double dTimeStep, double dSeed) {
	Duration dur;

	mt19937 rnd_gen2(dur.getCurrTime());   //Mersenne Twister generator
	//std::tr1::normal_distribution<double> normal;
	normal_distribution<> normal_dist;
	d_vGen2 = new rnd_normal_t(rnd_gen2, normal_dist);
	//rnd_poisson_t rnd_poisson( rnd_gen, boost::poisson_distribution<>( lambda ));

	urng = new MT(dSeed);
	Random::Set(*urng); 
	d_CurrVal = dSInitial;
	d_Drift = dDrift;
	d_Sigma = dSigma;
	d_Alpha = dMeanReversionSpeed;
	d_TimeStep = dTimeStep;
	d_d = 4 * d_Drift * d_Alpha / pow(d_Sigma, 2);
}

void CIRModel::invokePoisson(double dLambda)
{
	Duration dur;
	mt19937 rnd_gen1(dur.getCurrTime());   //Mersenne Twister generator
	d_vGen1 = new rnd_poisson_t( rnd_gen1, poisson_distribution<>( dLambda/2.0 ));
}

double CIRModel::step(void)
{
	//MT urng(dur.getCurrTime());                    // declare uniform random number generator
	//Random::Set(*urng);                   // set urng as generator to be used
   	
   	//for (int i=0; i<10; i++) cout << "chi: "<< CS.Next() << "\n";
   	double drift = pow(d_Sigma, 2) * (1 - exp((-1) * d_Alpha * d_TimeStep)) / (4 * d_Alpha);
	double d_Lambda = d_CurrVal * (exp((-1) * d_Alpha * d_TimeStep)) / drift;
   	
   	//printf("This is d: %f\n", d_d);
   	//fflush(stdout);
	if (d_d > 1){
		//printf("d is greater than 1\n");
		//fflush(stdout);
		double dRanNumber_Normal = (*d_vGen2)();
		//printf("Random Norm: %f\n", dRanNumber_Normal);
		//fflush(stdout);
		double degOfFreedom = d_d + 1;
		ChiSq CS1(degOfFreedom);
		double dRanNumber_Chi = CS1.Next();
		//printf("Random Chi: %f\n", dRanNumber_Chi);
		//fflush(stdout);
		d_CurrVal = drift * (pow((dRanNumber_Normal + sqrt(d_Lambda)),2) + dRanNumber_Chi);
		//printf("CurVal: %f\n", d_CurrVal);
		//fflush(stdout);
	} else {
		//printf("d is less than 1\n");
		//fflush(stdout);
		
		Duration dur;
		mt19937 rnd_gen1(dur.getCurrTime());   //Mersenne Twister generator
		d_vGen1 = new rnd_poisson_t( rnd_gen1, poisson_distribution<>( d_Lambda/2.0 ));
		
		//invokePoisson(d_Lambda);
		
		double dRanNumber_Poisson = (*d_vGen1)();
		
		while (dRanNumber_Poisson == 0.0)
			dRanNumber_Poisson = (*d_vGen1)();
			//TODO: WHat to do when poisson number is 0?
		/*printf("This is the ran no: %f\n", dRanNumber_Poisson); 
		fflush(stdout);*/
		
		double degOfFreedom = d_d + (2 * dRanNumber_Poisson);
		/*printf("This is the degree: %f\n", degOfFreedom); 
		fflush(stdout);*/
		ChiSq CS2(degOfFreedom);
		double dRanNumber_Chi = CS2.Next();
		
		d_CurrVal = drift * dRanNumber_Chi;
		delete d_vGen1;
	}
	
	return d_CurrVal;
}

CIRModel::~CIRModel() {
	delete d_vGen2;
}
