/*
 * MotionModel.h
 *
 *  Created on: May 04, 2019
 *      Author: linhtht
 */

#ifndef MOTIONMODEL_H__
#define MOTIONMODEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <limits>
#include <Eigen/Dense>	//Library for matrix and vector
#include "General.h"

using namespace Eigen;
using namespace std;

class MotionModel{
public:
	MotionModel(float a1, float a2, float a3, float a4 , bool useNormDist):
	_a1(a1),
	_a2(a2),
	_a3(a3),
	_a4(a4),
	_useNormDist(useNormDist){
	} 

	MotionModel(){
	}

	virtual ~MotionModel(){
	}

	//odPose: robot pose in odometry data coordinates
	Vector3f sampleMotionModelOdometry(Vector3f odPose_t, Vector3f odPose_t1, Vector3f pose_t1);
	Vector3f getOdomData(Vector3f odPose_t, Vector3f odPose_t1);
	Vector2f firstDerivativeProbNormalDistribution(float d, float b);
	Vector3f firstDerivativeLogMotionModelOdometry(Vector3f poseN, Vector3f odPose_t, Vector3f odPose_t1, Vector3f pose);

private:
	float _a1, _a2, _a3, _a4;
	bool _useNormDist;

	float sampleNormalDistribution(float b);
	float sampleTriangularDistribution(float b);
	float sample(float a);
};

#endif