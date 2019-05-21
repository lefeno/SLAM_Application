/*
 * Employee.h
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#ifndef MEDIATOR_EMPLOYEE_H_
#define MEDIATOR_EMPLOYEE_H_

#include "Mediator.h"
// #include "ServoController.h"
#include "json/json.h"
// #include "ScanData.h"
// #include "ServoController.h"
// #include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
// #include "RPLidarController.h"
// #include "OccuGridMap.h"
// #include "IncrementalMLMapping.h"
// #include "General.h"
// #include "MeasurementModel.h"
// #include "ScanData.h"

using namespace Eigen;
using namespace std;

class Mediator;
// class ServoController;

class Employee
{
public:
    Employee(Mediator* aMediator, int aSock);

	void start();

	virtual ~Employee();

private:
    static const int RCV_BUFFER_SIZE = 1024;
	int mSock;
	Mediator *mMediator;
    bool mSignal;
    // ServoController *mServoController;
    // Vector3f mPre;
    // Vector3f mCurr;

    // ScanData * mData;
    // MotionModel * mMotion;
    // MeasurementModel * mMesurement;
    // OccuGridMap * mGridMap;
    // RPLidarController * mRPlidar;

	void clientHdl();
    int sendToClient(const Json::Value& aRoot);
    int receivedCmdHandler(const char* aRcvMsg, int aRcvMsgSize);
    void cmdNotSupported(const std::string& aCmd);

    int moveServo(const Json::Value& aRoot);

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
    Json::Value mapJson(vector<Vector2f> coordinates);
};

#endif /* MEDIATOR_EMPLOYEE_H_ */
