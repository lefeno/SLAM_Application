/*
 * RPLidarController.h
 *
 *  Created on: May 03, 2019
 *      Author: linhtht
 */
// Add thickness of wall to the map??

#ifndef OCCUGRIDMAP_H__
#define OCCUGRIDMAP_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <limits>
#include <Eigen/Dense>	//Library for matrix and vector
#include <vector>

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

#include "ScanData.h"

using namespace Eigen;
using namespace std;
// using namespace cv;

class OccuGridMap{
public:
	OccuGridMap(){
	_mapResolution = 0;
	_mapSizeX = 0;
	_mapSizeY = 0;
	_totalMapSizeX = 0;
	_totalMapSizeY = 0;
	_alpha = 0;
	_l0 = _l_occ = _l_free = 0;
	_robotSize = 1;
	_enableTest = false;
	}

	virtual ~OccuGridMap(){
	}

/*
 *  Create a map with resolution: mapResolution
 *	Size: mapSizeX*mapSizeY grids
 * 	alpha: the thickness of the obstacles
 *	beta: the angle between 2 neighbor beams
 * 	l0: value of a "don't-care" grid
 *	l_occ: value of an occupied grid
 *	l_free: value of a free grid
 *	robotSize: smallest edge of robot 
 *	enableTest: enable opencv to display images/videos
 */

	void createMap(float mapResolution,
		int mapSizeX,
		int mapSizeY,
		float alpha,
		float l0,
		float l_occ,
		float l_free,
		float robotSize,
		bool enableTest = false);

	//pose: robot pose in global coordinate
	//sensorPose: sensor position in robot coordinate default: line at (0,0) of robot
	// update the scan area only
	void updateMap(Vector3f pose, ScanData * scanData, Vector2f sensorPose = Vector2f(0,0));

	// project the beam onto map, treat free grids = don't-care grids, don't update the previous value ->%failure??
	void updateMapFast(Vector3f pose, ScanData * scanData, Vector2f sensorPose = Vector2f(0,0));

	MatrixXd getMap(){
		return _map;
	}

	int getMapSizeX(){
		return _mapSizeX;
	}

	int getMapSizeY(){
		return _mapSizeY;
	}

	void setMap(MatrixXd map){
		_map = map;
	}

	vector<Vector2f> getOccGrids(){
		return _occ;
	}

	void clean(){
		_map = MatrixXd::Constant(_mapSizeX, _mapSizeY, _l0);
	}
private:
	bool _enableTest;

	float _mapResolution;
	int _mapSizeX;
	int _mapSizeY;
	int _totalMapSizeY;
	int _totalMapSizeX;
	MatrixXd _map;
	// current pose of robot in global coordinate (x,y,theta)
	// theta is angle between robot direction and x-axis (-pi,pi)
	Vector3f _initialPose;
	float _alpha;
	float _l0;
	float _l_occ;
	float _l_free;
	float _robotSize;
	vector<Vector2f> _occ;
	// Mat1f _image;

	// consider grid(i,j)
	void update(int i, int j, ScanData* data);
};

#endif