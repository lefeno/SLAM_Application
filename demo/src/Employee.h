/*
 * Employee.h
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#ifndef MEDIATOR_EMPLOYEE_H_
#define MEDIATOR_EMPLOYEE_H_

#include<fstream>
#include "Mediator.h"
#include "ServoController.h"
#include "json/json.h"
#include "ScanData.h"
#include "ServoController.h"
#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include "OccuGridMap.h"
#include "IncrementalMLMapping.h"
#include "General.h"
#include "MeasurementModel.h"
#include "ScanData.h"
#include <algorithm>

using namespace Eigen;
using namespace std;

class Mediator;
class ServoController;

class Employee
{
public:
    Employee(Mediator* aMediator, ServoController *aServoController, RPLidarController * aRPlidar, int aSock);

	void start();

	virtual ~Employee();

private:
    static const int RCV_BUFFER_SIZE = 1024;
	int mSock;
	Mediator *mMediator;
    ServoController *mServoController;
    RPLidarController *mRPLidar;

	void clientHdl();
    int sendToClient(const Json::Value& aRoot);
    void cmdNotSupported(const std::string& aCmd);

    // int moveServo(const Json::Value& aRoot);
    int moveServo(const Json::Value &aRoot, bool &trigger, int & timer, Vector3f & control);
    int receivedCmdHandler(const char *aRcvMsg, int aRcvMsgSize, bool &trigger, int & timer, Vector3f & control);

	/**
	 * Handle Get Robot Info command
	 * @param aRoot The Json of the incoming command
	 * @return 0 if success
	 */
	int getRobotInfo(const Json::Value& aRoot);

	/**
	 * Handle Get Servo Info command
	 * @param aRoot The Json of the incoming command
	 * @return 0 if success
	 */
	int getServoInfo(const Json::Value& aRoot);

    /**
     * @brief Handle session initiation command
     * @return 0 if success, negative otherwise.
     */
    int sessionInitiation();

	/**
     * @brief Convert occupied grids to Json
     */

    // convert rpm value: 400-6000 to velocity value: 20-100
    double rpmToV(short rpm);
    int getStreamingInfo(const Json::Value &aRoot);
};

#endif /* MEDIATOR_EMPLOYEE_H_ */
