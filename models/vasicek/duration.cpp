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

#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "duration.h"
//#include <iostream>

//using namespace std;

void Duration::setStart()
{
	gettimeofday(&tvStart, NULL);
	tmStart = localtime(&tvStart.tv_sec);
}

void Duration::setEnd()
{
        gettimeofday(&tvEnd, NULL);
        tmStart = localtime(&tvEnd.tv_sec);
}

double Duration::getPassedTime()
{
	double dStart = (double)tvStart.tv_sec + (double)tvStart.tv_usec/1000000.0;
	double dEnd = (double)tvEnd.tv_sec + (double)tvEnd.tv_usec/1000000.0;
	return dEnd - dStart;
}

long int Duration::getCurrTime()
{
	gettimeofday(&tvEnd, NULL);
        tmStart = localtime(&tvEnd.tv_sec);
        long int dEnd = tvEnd.tv_sec * 1000000 + tvEnd.tv_usec;
	return dEnd;
}

char* Duration::getSysTime()
{
	time_t rawtime;
  	time ( &rawtime );
  	char *czCurTime = new char[128];
  	sprintf(czCurTime, "%s",ctime (&rawtime));
  	
  	return czCurTime;
}
//test programme
/*int main(int argc, char* argv[])
{
	Duration du;
	du.setStart();
	sleep(5);
	du.setEnd();
	cout << du.getPassedTime() << endl;

	return 0;
}
*/

