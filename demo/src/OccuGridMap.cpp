/*
 * RPLidarController.h
 *
 *  Created on: May 03, 2019
 *      Author: linhtht
 */

#include "OccuGridMap.h"

void OccuGridMap::createMap(float mapResolution,	//map resolution is considered to be cm
		int mapSizeX,
		int mapSizeY,
		float alpha,
		float l0,
		float l_occ,
		float l_free,
		float robotSize,
		bool enableTest){
	_mapResolution = mapResolution;
	_mapSizeX = mapSizeX;
	_mapSizeY = mapSizeY;
	_totalMapSizeX = _mapSizeX * _mapResolution;
	_totalMapSizeY = _mapSizeY * _mapResolution;;
	_alpha = alpha;
	_l0 = l0;
	_l_occ = l_occ;
	_l_free = l_free;
	_map = MatrixXd::Constant(_mapSizeX, _mapSizeY, _l0);
	_initialPose(0) = 0.5 * _totalMapSizeX;
	_initialPose(1) = 0.5 * _totalMapSizeY;
	_robotSize = robotSize;		
	_enableTest = enableTest;
	// if(_enableTest){
	// 	_image = Mat1f(_mapSizeY,_mapSizeX);
	// 	for(int i = 0; i < _mapSizeY; ++i){
	// 		for(int j = 0; j < _mapSizeX; j++){
	// 			_image(i,j) = 255;
	// 		}
	// 	}
	// }
}

// project the beam onto map, treat free grids = don't-care grids, don't update the previous value too->%failure??
void OccuGridMap::updateMapFast(Vector3f pose, ScanData* data, Vector2f sensorPose){
	int x_s, y_s;
	float theta_bl;
	for(int i = 0; i < data->nodeCount; ++i){
		if(data->ranges[i] >= data->rangeMax){
			continue;
		}
		// project the beam onto map
		theta_bl = (i + 1) * data->angleIncrement;
		if(theta_bl > M_PI){
			theta_bl -= 2*M_PI;
		}
		x_s = pose[0] + sensorPose[0] * cos(pose[2]) - sensorPose[1] * sin(pose[2]) + data->ranges[i] * cos(pose[2] + theta_bl);
		y_s = pose[1] + sensorPose[1] * cos(pose[2]) + sensorPose[0] * sin(pose[2]) + data->ranges[i] * sin(pose[2] + theta_bl);

		// _image(x_s, y_s) = _l_occ;
		_occ.push_back(Vector2f(x_s,y_s));
	}
	
	if(_enableTest){
		// imwrite("map.jpg",_image);	
	}
}

//should I update the entire map?? or just a part of map
void OccuGridMap::updateMap(Vector3f pose, ScanData* data, Vector2f sensorPose){
	//reset occupid grids before
	
	_occ.clear();

	if(pose(0) > _totalMapSizeX || pose(1) > _totalMapSizeY || pose(0) < 0 || pose(1) < 0){
		cout << "Out of the map" << endl;
		return;
	}
	_initialPose = pose;

	// find 4 impotant coordinates
	int xLeft, xRight, xMidLeft, xMidRight, yBot, yTop, yMidBot, yMidTop; 
	
	float tempRange = data->rangeMax + _mapResolution*0.5;
	xLeft = ((_initialPose(0)-tempRange) > 0 ? (_initialPose(0)-tempRange) : 0)/_mapResolution;
	yBot = ((_initialPose(1)-tempRange) > 0 ? (_initialPose(1)-tempRange) : 0)/_mapResolution;
	xRight = ((_initialPose(0)+tempRange) < _totalMapSizeX ? (_initialPose(0)+tempRange) : (_totalMapSizeX - _mapResolution*0.5))/_mapResolution;
	yTop = ((_initialPose(1)+tempRange) < _totalMapSizeY ? (_initialPose(1)+tempRange) : (_totalMapSizeY - _mapResolution*0.5))/_mapResolution;
	
	tempRange = (_robotSize*0.5 + data->rangeMin)*0.707 - _mapResolution*0.5;
	xMidLeft = ((_initialPose(0)-tempRange) > 0 ? (_initialPose(0)-tempRange) : 0)/_mapResolution;
	yMidBot = ((_initialPose(1)-tempRange) > 0 ? (_initialPose(1)-tempRange) : 0)/_mapResolution;
	xMidRight = ((_initialPose(0)+tempRange) < _totalMapSizeX ? (_initialPose(0)+tempRange) : (_totalMapSizeX - _mapResolution*0.5))/_mapResolution;
	yMidTop = ((_initialPose(1)+tempRange) < _totalMapSizeY ? (_initialPose(1)+tempRange) : (_totalMapSizeY - _mapResolution*0.5))/_mapResolution;
	
	for(int i = xLeft; i < xMidLeft; ++i){
		for(int j = yBot; j < yTop; ++j){
			this->update(i,j,data);
		}
	}
	
	for(int i = xMidRight;i < xRight; ++i){
		for(int j = yBot; j < yTop; ++j){
			this->update(i,j,data);	
		}
	}		
	
	for(int i = xMidLeft; i < xMidRight; ++i){
		for(int j = yBot; j < yMidBot; ++j){
			this->update(i,j,data);
		}
		for(int j = yMidTop; j < yTop; ++j){
			this->update(i,j,data);
		}
	}
	if(_enableTest){
		// imwrite("map.jpg",_image);	
	}
	
}

void OccuGridMap::update(int i, int j, ScanData* data){
	Vector2f mi;
	mi(0) = (i+1)*_mapResolution - _mapResolution*0.5;
	mi(1) = (j+1)*_mapResolution - _mapResolution*0.5;
	
	double r = 0;
	r = sqrt((mi(0) - _initialPose(0))*(mi(0) - _initialPose(0)) + (mi(1) - _initialPose(1))*(mi(1) - _initialPose(1)));

	if ( r > data->rangeMax ){
		if(_enableTest){
			// _image(i,j) = _map(i,j);
		}
		return;
	}
	
	float temp = 0.0;
	// Calculate the angle between beam and the robot direction
	float phi = atan2(mi(1)-_initialPose(1),mi(0)-_initialPose(0)) - _initialPose(2);
	if(phi < 0){
		phi += 2*M_PI;
	}
	
	int k = phi/data->angleIncrement, k0, k1;
	k0 = k;
	k1 = (k+1)%((int)(2*M_PI/data->angleIncrement));
	if(abs((k0*data->angleIncrement) - phi) > abs(((k1)*data->angleIncrement) - phi)){
		k = k1;
	} else {
		k = k0;
	}
	
	// printf("Goc phi: %f, k: %d\n", phi*180/M_PI, k);
	float k_angle = k*data->angleIncrement;
	if (r > min((float)data->rangeMax, (float)(data->ranges[k] + _alpha*0.5)) || (abs(phi - k_angle) > data->angleIncrement*0.5)){
		temp = _l0;
	}
	else if (data->ranges[k] < data->rangeMax && abs(r - data->ranges[k]) < _alpha*0.5){
		temp = _l_occ;
		_occ.push_back(Vector2f(i,j));
	}
	else if (r <= data->ranges[k]){
		temp = _l_free;
	}				
	
	//??Tai sao lai co gia tri -122,0
	if(i > 0 && i < _mapSizeX && j > 0 && j < _mapSizeY){
		_map(i,j) += temp - _l0;
	}

	if(_enableTest){
		// _image(i,j) = _map(i,j);
	}
}