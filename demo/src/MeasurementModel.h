/*
 * MeasurementModel.h
 *
 *  Created on: May 05, 2019
 *      Author: linhtht
 */
#ifndef MEASUREMENTMODEL_H__
#define MEASUREMENTMODEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include "OccuGridMap.h"
#include "ServoController.h"
#include "MotionModel.h"
#include "General.h"
#include "json/json.h"
#include <vector>

#include <math.h>
#include <sys/time.h>
#include <Eigen/Dense>	//Library for matrix and vector
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace Eigen;
using namespace std;

class MeasurementModel{
public:
	MeasurementModel(){
	}

	MeasurementModel(float zhit, float zrand, float sig_hit, float zmax){
		_zhit = zhit;
		_zrand = zrand;
		_sig_hit = sig_hit;
		_zmax = zmax;
	}

	virtual ~ MeasurementModel(){
	}

	float measurementModel(ScanData * data, Vector3f pose, vector<Vector2f> occ, Vector2f sensorPose = Vector2f(0,0));
	Vector3f firstDerivativeLogRangeFinderModel(ScanData* data, Vector3f pose, vector<Vector2f> occ, Vector2f sensorPose = Vector2f(0,0));
	
private:
	float _zhit;
	float _zrand;
	float _sig_hit;
	float _zmax;
};

#endif