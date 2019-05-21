/*
 * Mediator.h
 *
 *  Created on: Feb 22, 2017
 *      Author: trucndt
 */

#ifndef MEDIATOR_MEDIATOR_H_
#define MEDIATOR_MEDIATOR_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <math.h>
#include <sys/time.h>
#include <Eigen/Dense>  //Library for matrix and vector
#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "Employee.h"
#include "utils.h"
#include "ServoController.h"
#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include "OccuGridMap.h"
#include "IncrementalMLMapping.h"
#include "General.h"
#include "MeasurementModel.h"
#include "ScanData.h"

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

extern boost::program_options::variables_map gPROG_ARGUMENT;

using namespace Eigen;
using namespace std;

class Employee;

class Mediator
{
public:
	Mediator();

	void start();

    /**
     * @brief comparePassword
     * @param aPass
     * @return true if match
     */
    bool comparePassword(const std::string &aPass);

    int getCountNode(){
        return mNodeCount;
    };

	virtual ~Mediator();

    vector<int> mClients;

    string getIP(){
        return mIP;
    }

    uint16_t getServerPort(){
        return mServPort;
    }

    void exportToFile(const Json::Value &aRoot);

private:
	uint16_t mServPort;
    string mIP;

    std::string mPassword;
    int mNodeCount;

    OccuGridMap * mGridMap;
    ServoController* mServoController;
    RPLidarController *mRPlidar;

    // save the Previous and Current pose from Odometer
    Vector3f mPre;
    Vector3f mCurr;

    ScanData * mData;
    MotionModel * mMotion;
    MeasurementModel * mMesurement;

	const static unsigned int MAXPENDING = 5;    /* Maximum outstanding connection requests */

    //Handle map
    void mapHdl();
    //convert coordinates to Json
    Json::Value mapJson(vector<Vector2f> coordinates); 

    int sendToClient(const Json::Value &aRoot);
};

#endif /* MEDIATOR_MEDIATOR_H_ */
