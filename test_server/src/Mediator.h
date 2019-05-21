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

#include <boost/thread/thread.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include "Employee.h"
#include "utils.h"
// #include "ServoController.h"
// #include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
// #include "RPLidarController.h"
// #include "OccuGridMap.h"
// #include "IncrementalMLMapping.h"
// #include "General.h"
// #include "MeasurementModel.h"
// #include "ScanData.h"

// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>

using namespace Eigen;
using namespace std;

extern boost::program_options::variables_map gPROG_ARGUMENT;

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
        return _countNode;
    };

	virtual ~Mediator();

    void mapHdl();
    Json::Value mapJson(vector<Vector2f> coordinates);
private:
	uint16_t mServPort;
    std::string mPassword;
    int _countNode;

    // OccuGridMap * mGridMap;
    // ServoController* mServoController;
    // RPLidarController *mRPlidar;

	const static unsigned int MAXPENDING = 5;    /* Maximum outstanding connection requests */
};

#endif /* MEDIATOR_MEDIATOR_H_ */
