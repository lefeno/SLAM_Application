/*
 * IncrementalMLMapping.h
 *
 *  Created on: May 05, 2019
 *      Author: linhtht
 */
#ifndef INCREMENTALMLMAPPING_H__
#define INCREMENTALMLMAPPING_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include "OccuGridMap.h"
#include "ServoController.h"
#include "MotionModel.h"
#include "MeasurementModel.h"
#include "General.h"
#include "json/json.h"
#include <vector>

#include <math.h>
#include <sys/time.h>
#include <Eigen/Dense>	//Library for matrix and vector
#include <algorithm>

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

using namespace Eigen;
using namespace std;

class IncrementalMLMapping{
public:
	//odPose: robot pose in odometry data coordinates
	IncrementalMLMapping(MotionModel * motion, MeasurementModel * measurement, float aSlow, float aFast, bool useNormDist, float sizeXt)
	{
		//?? what is the initial value of those 2
		_aSlow = aSlow;
		_aFast = aFast;
		_useNormDist = useNormDist;
		_motion = motion;
		_measurement = measurement;
		_wSlow = 0;
		_wFast = 0;
		_sizeXt = sizeXt;
	} 

	virtual ~IncrementalMLMapping(){
	}

	void augmented_MCL(Vector3f xt, Vector3f xt1, ScanData* zt, OccuGridMap * m);
	Vector3f generateRandPose(OccuGridMap * m);
	Vector3f getPossiblePose();
	void createRandXt(OccuGridMap * m);
	Vector3f incrementalMLMapping(ScanData* data, Vector3f poseN, Vector3f odPost_t, Vector3f odPose_t1, OccuGridMap * m);

private:
	float _sizeXt;
	vector<Vector3f> _Xt1;
	vector<Vector3f> _Xt;
	MotionModel* _motion;
	MeasurementModel* _measurement;
	bool _useNormDist;
	float _wSlow, _wFast, _aSlow, _aFast;
};

#endif