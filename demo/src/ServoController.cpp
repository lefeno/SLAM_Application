/*
 * ServoController.cpp
 *
 *  Created on: Feb 24, 2017
 *      Author: baohq
 */

#include "ServoController.h"

IesSdk_ReturnValue_t ServoController::initialize(int coordinateSys)
{
	IesSdk_ReturnValue_t servoReturnVal;

	servoReturnVal = Servo_initialize(coordinateSys);
	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_initialize failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}
	return servoReturnVal;
}

IesSdk_ReturnValue_t ServoController::moveForward(double movingThreshold, double velocity)
{
	IesSdk_ReturnValue_t servoReturnVal;
	servoReturnVal = initialize(DEFAULT_COORDINATE_SYSTEM);

	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_move_direct failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}
	servoReturnVal = Servo_move_direct(movingThreshold, 0, velocity);

	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_set_rpm failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}

	return servoReturnVal;
}

IesSdk_ReturnValue_t ServoController::moveXY(double x, double y, double velocity)
{
	IesSdk_ReturnValue_t servoReturnVal;
	// servoReturnVal = initialize(DEFAULT_COORDINATE_SYSTEM);

	// if (servoReturnVal != SUCCESS)
	// {
	// 	cout << "Servo_move_direct failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
	// 	return servoReturnVal;
	// }
	cout << "x: " << x << "y: " << y << "velocity: " << velocity <<  endl;
	servoReturnVal = Servo_move_direct(x, y, velocity);

	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_set_rpm failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}

	return servoReturnVal;
}

IesSdk_ReturnValue_t ServoController::moveBackward(long movingTime, double velocity)
{
	short absRpmVal;
	IesSdk_ReturnValue_t servoReturnVal;

	absRpmVal = velocityToRpm(velocity);
	servoReturnVal = Servo_set_rpm(-absRpmVal, absRpmVal);

	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_set_rpm failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}
	usleep(movingTime);
	return stopRobot();
}

IesSdk_ReturnValue_t ServoController::turn(double radThreshold, double velocity)
{
	IesSdk_ReturnValue_t servoReturnVal;

	servoReturnVal = Servo_rotate(radThreshold, velocity);
	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_rotate failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}
	return servoReturnVal;
}

IesSdk_ReturnValue_t ServoController::stopRobot()
{
	IesSdk_ReturnValue_t servoReturnVal = Servo_set_rpm(-STOP_RANGE_ABSVALUE_RPM, STOP_RANGE_ABSVALUE_RPM);
	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_set_rpm failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return servoReturnVal;
	}

	return servoReturnVal;
}

/*
 * Base on formula
 * max range: [-6000                  [-500**Stop range**500]                        6000]
 * => valid range for movement: [-6000  -550 --- 550  6000]
 * Valid velocity: 20 - 100, each increase 0.1% => 800 levels
 * => Each level ~ (6000 - 550)/800 ~ 6.8rpm
 * ==> Final formula: 550 + (6000 - 550)*(velocity - 20)/80
 */
short ServoController::velocityToRpm(double velocity)
{
	if (velocity < MIN_VELOCITY || velocity > MAX_VELOCITY)
	{
		cout << "Invalid velocity." << endl;
		return STOP_RANGE_ABSVALUE_RPM;
	}

	double rpm = 550 + VALID_MOVEMENT_RPM * (velocity - 20)/80;

	return (short)rpm;
}

Vector3f ServoController::getPose(){
	Vector3f pose(0,0,0); //pose invalid
	IesSdk_ReturnValue_t servoReturnVal;
	Servo_Position_t pos;

	cout << "Getting pose ...\n";

	servoReturnVal = Servo_get_odometer(&pos);

	if (servoReturnVal != RUNNING && servoReturnVal != STANDBY)
	// if (servoReturnVal != STANDBY)
	{
		cout << "Servo_get_odometer failed from " << __FILE__ << __LINE__ << endl << "Error Code " << servoReturnVal << endl;
		return pose;
	}
	pose(0) = pos.y*0.1;
	pose(1) = pos.x*0.1;
	if(pos.rad < -M_PI*0.5 && pos.rad > -M_PI){
		pose(2) = -pos.rad - M_PI*0.5*3;
	} else {
		pose(2) = M_PI*0.5 - pos.rad;
	}

	return pose;
}

Json::Value ServoController::getOdometer()
{
	Json::Value root;
	IesSdk_ReturnValue_t servoReturnVal;
	Servo_Position_t pos;

	cout << "Step into getOdometer\n";

	servoReturnVal = Servo_get_odometer(&pos);

	// if (servoReturnVal != RUNNING && servoReturnVal != STANDBY)
	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_get_odometer failed from " << __FILE__ << __LINE__ << endl << "Error Code " << servoReturnVal << endl;
		return root;
	}

	root["Status"] = servoReturnVal;
	// Need to change the coordinates
	// Original coordinates
	// root["Position"]["x_coordinate"] = pos.x;
	// root["Position"]["y_coordinate"] = pos.y;
	// root["Position"]["rad"] = pos.rad;
	// Converted coordinates
	root["Position"]["x_coordinate"] = pos.y;
	root["Position"]["y_coordinate"] = pos.x;
	if(pos.rad < -M_PI*0.5 && pos.rad > -M_PI){
		root["Position"]["rad"] = -pos.rad - M_PI*0.5*3;
	} else {
		root["Position"]["rad"] = M_PI*0.5 - pos.rad;
	}

	return root;
}

Json::Value ServoController::getServoInfo()
{
	Json::Value root;
	IesSdk_ReturnValue_t servoReturnVal;
	Servo_Info_t servoInfo;

	servoReturnVal = Servo_get_info(&servoInfo);

	if (servoReturnVal != SUCCESS)
	{
		cout << "Servo_get_info failed from " << __FILE__ << __LINE__ << "Error Code " << servoReturnVal << endl;
		return root;
	}

	/*
	 * Control Information:
	 * Speed
	 * Acceleration
	 * Motors: left motor - motors[0], right motor - motors[1]
	 */
	root["Control Info"]["Speed"] = servoInfo.control.speed;
	root["Control Info"]["Acc"] = servoInfo.control.acc;
	root["Control Info"]["Motors"]["LeftMotor"]["TargetRpm"] = servoInfo.control.motors[0].target_rpm;
	root["Control Info"]["Motors"]["LeftMotor"]["DetectionRpm"] = servoInfo.control.motors[0].detection_rpm;
	root["Control Info"]["Motors"]["RightMotor"]["TargetRpm"] = servoInfo.control.motors[1].target_rpm;
	root["Control Info"]["Motors"]["RightMotor"]["DetectionRpm"] = servoInfo.control.motors[1].detection_rpm;

	/*
	 * Performance Information:
	 * CPU performance
	 * RAM performance
	 * Network Performance
	 */

	/*
	 * CPU performance
	 */
	root["Performance Info"]["CPU"]["Task 0"] = servoInfo.performance.cpu.utilization[0];
	root["Performance Info"]["CPU"]["Task 1"] = servoInfo.performance.cpu.utilization[1];
	root["Performance Info"]["CPU"]["Task 2"] = servoInfo.performance.cpu.utilization[2];
	root["Performance Info"]["CPU"]["Task 3"] = servoInfo.performance.cpu.utilization[3];

	/*
	 * RAM performance
	 */
	root["Performance Info"]["RAM"]["Allocated"] = servoInfo.performance.ram.allocated;
	root["Performance Info"]["RAM"]["Total"] = servoInfo.performance.ram.total;

	/*
	 * Network performance
	 */
	root["Performance Info"]["Network"]["Communication 0"]["Transmitted"] = servoInfo.performance.network[0].transmitted;
	root["Performance Info"]["Network"]["Communication 0"]["Received"] = servoInfo.performance.network[0].received;

	root["Performance Info"]["Network"]["Communication 1"]["Transmitted"] = servoInfo.performance.network[1].transmitted;
	root["Performance Info"]["Network"]["Communication 1"]["Received"] = servoInfo.performance.network[1].received;

	root["Performance Info"]["Network"]["Communication 2"]["Transmitted"] = servoInfo.performance.network[2].transmitted;
	root["Performance Info"]["Network"]["Communication 2"]["Received"] = servoInfo.performance.network[2].received;

	/*
	 * Diagnosis information:
	 * Failed packets
	 * Total packet
	 */

	root["Diagnosis"]["Network"]["Communication 0"]["Failed"] = servoInfo.diagnosis.network[0].failed;
	root["Diagnosis"]["Network"]["Communication 0"]["Total"] = servoInfo.diagnosis.network[0].times;

	root["Diagnosis"]["Network"]["Communication 1"]["Failed"] = servoInfo.diagnosis.network[1].failed;
	root["Diagnosis"]["Network"]["Communication 1"]["Total"] = servoInfo.diagnosis.network[1].times;

	root["Diagnosis"]["Network"]["Communication 2"]["Failed"] = servoInfo.diagnosis.network[2].failed;
	root["Diagnosis"]["Network"]["Communication 2"]["Total"] = servoInfo.diagnosis.network[2].times;

	return root;
}

IesSdk_ReturnValue_t ServoController::setRpm(short right, short left)
{
    IesSdk_ReturnValue_t servoReturnVal;

    servoReturnVal = Servo_set_rpm(right, left);

    if (servoReturnVal != SUCCESS)
    {
        cout << "Servo_set_rpm failed from " << __FILE__ << ":" << __LINE__
			 << "\nError Code " << servoReturnVal << endl;
    }

	return servoReturnVal;
}
