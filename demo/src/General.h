/*
 * General.h: contains shared functions
 *
 *  Created on: May 04, 2019
 *      Author: linhtht
 */

#ifndef GENERAL_H__
#define GENERAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <limits>
#include <Eigen/Dense>	//Library for matrix and vector

using namespace Eigen;
using namespace std;

class General{
public:
	General(){
	}
	virtual ~ General(){
	}

	static void ready(){
		srand((unsigned int)time(NULL));	
	}

	static float genRandNumber(int a, int b){
		return ((float)rand()/(float)(RAND_MAX))*abs(b-a) + a;
	}

	static float prob(float a, float b, bool useNormDist = true){
		if(useNormDist){
			return 1.0/sqrt(2*M_PI*b)*exp(-0.5*a*a/b);
		} else{
			if(abs(a) > sqrt(6*b)) return 0;
			return (sqrt(6*b) - abs(a))/(6.0*b);
		}
	}

};

#endif