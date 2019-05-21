/*
 * ServoController.h
 *
 *  Created on: Feb 24, 2017
 *      Author: baohq
 */

#ifndef MEDIATOR_SERVOCONTROLLER_H_
#define MEDIATOR_SERVOCONTROLLER_H_

//#include <Mediator.h>
#include "json/json.h"
#include <app/inc/IesSdk.h>
#include <app/inc/Servo.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <Eigen/Dense>	//Library for matrix and vector

#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "utils.h"

using namespace Eigen;
using namespace std;

class ServoController
{
public:
	ServoController()
	{

	}

	virtual ~ServoController()
	{

	}

    //TODO blocking methods
	IesSdk_ReturnValue_t initialize(int coordinateSys);
	IesSdk_ReturnValue_t moveForward(double movingthreshold, double velocity);
	IesSdk_ReturnValue_t moveXY(double x, double y, double velocity);
	IesSdk_ReturnValue_t moveBackward(long movingTime, double velocity);
	IesSdk_ReturnValue_t turn(double radThreshold, double velocity);
	IesSdk_ReturnValue_t setRpm(short right, short left);
	IesSdk_ReturnValue_t stopRobot();
	Json::Value getServoInfo();
	Json::Value getOdometer();
	Vector3f getPose(); // return (-1,-1,-1) is invalid pose

	static const double MIN_MOVEMENT_THRESHOLD = 150.0;	//in mm
	static const double MIN_ROTATION_THRESHOLD = 0.001; 	//in radian
	static const double MAX_VELOCITY = 100.0;				//in %
	static const double MIN_VELOCITY = 20.0;				//in %
	static const short MAX_ABSVALUE_RPM = 6000;			//round per minute
	static const short STOP_RANGE_ABSVALUE_RPM = 400;		//round per minute
	static const short VALID_MOVEMENT_RPM = MAX_ABSVALUE_RPM - 550;
	static const int DEFAULT_COORDINATE_SYSTEM = 0;
private:
	short velocityToRpm(double velocity);
};

#endif /* MEDIATOR_SERVOCONTROLLER_H_ */
