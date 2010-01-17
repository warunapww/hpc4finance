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

/*The exact solution of the Stochastic Differential Equation has been used
refer: http://www.sitmo.com/eq/166
There is a conflict  with the equation given in http://books.google.lk/books?id=nh6zHOf9tn0C&pg=PA63&lpg=PA63&dq=exact+solution+of+the+geometric+brownian+motion+SDE&source=web&ots=ZU7ooKCvwj&sig=DWnt0a4SGwloey70AC4FBtPmz-g&hl=en&sa=X&oi=book_result&resnum=5&ct=result#PPA65,M1
The difference is the presence of a sqrt(t) term in the sitmo article.
*/

#include <stdio.h>
#include "GBMotion.h"
#include <math.h>
#include <cstdlib>
#include <ctime>
#include "duration.h"

using namespace boost;
using namespace std;

CGBMotion::CGBMotion(double nSInitial, double nDrift, double nSigma, double deltaT, double dSeed)
{
	m_nCurrValue = nSInitial;
	m_nDrift = nDrift;
	m_nSigma = nSigma;
	m_nDeltaT = deltaT;
	//m_nCurrentDiffusion = 0;
	static mt19937 engine(static_cast<unsigned> (dSeed));
	normal_distribution<> normal_dist;
	d_vGen = new variate_generator<mt19937&, normal_distribution<double> >(engine, normal_dist);
}

CGBMotion::~CGBMotion(void)
{
	delete d_vGen;
}

double CGBMotion::step()
{
		double random_number = (*d_vGen)(); 
		//cout << "Ran No: " << random_number << endl;
		double m_nCurrentDiffusion = sqrt(m_nDeltaT) * random_number;
		m_nCurrValue *= exp(m_nDrift*m_nDeltaT - .5* m_nSigma * m_nSigma*m_nDeltaT 
			+ m_nSigma*m_nCurrentDiffusion);

		return m_nCurrValue;
		
}
