/*
 * RPLidarController.h
 *
 *  Created on: April 07, 2019
 *      Author: linhtht
 */

#ifndef RPLIDARCONTROLLER_H__
#define RPLIDARCONTROLLER_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cstring>
#include <limits>

#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header

#include "ScanData.h"
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#define DEG2RAD(x) ((x)*M_PI/180.)

using namespace rp::standalone::rplidar;

class RPLidarController{
public:
	RPLidarController(){
		_drv = NULL;
		_optComPath = NULL;
		_baudrate = 0;
		_data = NULL;
	}
	virtual ~RPLidarController(){

	}
	
	bool checkRPLIDARHealth();
	// default scan rate is 4k, mode = 1
	int initialize(const char * optComPath = "/dev/ttyUSB0", _u32 baudrate = 115200, _u16 mode = 1, float rangeMin = 15, float rangeMax = 300);
	bool getRPLIDARDeviceInfo();
	bool stopMotor();
	bool startMotor();

	void getScanData(
		rplidar_response_measurement_node_hq_t * nodes,				
		size_t nodeCount, 
		// int scanTime,
		bool inverted,
		float angleMin, 
		float angleMax);
	// As now, we just test with countNodes = 360 or 720
	ScanData * getScanArray(int countNodes = 360, bool inverted = false, bool angleCompensate = true);
	void reset();
	void clean();
private:
	RPlidarDriver * _drv;
	const char * _optComPath;
	_u32 _baudrate;
	ScanData * _data;	
	_u16 _mode;	
};
#endif /* RPLIDARCONTROLLER_H__ */
