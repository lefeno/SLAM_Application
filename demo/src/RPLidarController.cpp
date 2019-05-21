/*
 * RPLidarController.h
 *
 *  Created on: April 07, 2019
 *      Author: linhtht
 */
#include "RPLidarController.h"

using namespace rp::standalone::rplidar;

bool RPLidarController::checkRPLIDARHealth(){
	u_result opResult;
	rplidar_response_device_health_t healthInfo;
			
	opResult = _drv->getHealth(healthInfo);
	if (IS_OK(opResult)){// the macro IS_OK is the preperred way to judge whether the operation is succeed.
     	 	printf("RPLidar health status : %d\n", healthInfo.status);
		if (healthInfo.status == RPLIDAR_STATUS_ERROR) {
			fprintf(stderr, "Error, rplidar internal error detected. Please reboot the device to retry.\n");
			return false;
		} else {
			return true;
		}
	} else {
		fprintf(stderr, "Error, cannot retrieve the lidar health code: %x\n", opResult);
        	return false;
	}
}

int RPLidarController::initialize(const char * optComPath, _u32 baudrate, _u16 mode, float rangeMin, float rangeMax){
	_optComPath = optComPath;
	_baudrate = baudrate;
	_drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
			
	if(!_drv) {
		fprintf(stderr, "insufficent memory, exit\n");
		clean();
	        return -2;
	}
	//make connection...
	if (IS_FAIL(_drv->connect(_optComPath, _baudrate))) {
     		fprintf(stderr, "Error, cannot bind to the specified serial port %s.\n", _optComPath);
	        clean();	
		return -1;
	}
		
	// get rplidar device info
	if(!getRPLIDARDeviceInfo()){
		clean();
		return -1;
	}

	// check health ...
	if(!checkRPLIDARHealth()){
		clean();
		return -1;
	}
	
	_data = new ScanData(rangeMin,rangeMax,0,0,0,NULL,NULL,360);
	_mode = mode;
	return 0;
}

bool RPLidarController::getRPLIDARDeviceInfo(){
	u_result opResult;
	rplidar_response_device_info_t devinfo;
	opResult = _drv->getDeviceInfo(devinfo);

	if(IS_FAIL(opResult)){
		if(opResult == RESULT_OPERATION_TIMEOUT){
			fprintf(stderr, "Error, operation time out.\n");
		} else {
			fprintf(stderr, "Error, unexpected error, code: %x\n", opResult);
		}
		clean();
		return false;
	}

	// print out the device serial number, firmware and hardware version number
	printf("RPLIDAR S/N: ");
	for (int pos = 0; pos < 16; ++pos) {
		printf("%02X", devinfo.serialnum[pos]);
	}
	printf("\n"
           "Firmware Ver: %d.%02d\n"
           "Hardware Rev: %d\n"
           , devinfo.firmware_version>>8
           , devinfo.firmware_version & 0xFF
           , (int)devinfo.hardware_version);
	return true;
}

bool RPLidarController::stopMotor(){
	if(!_drv){
		clean();	
		return false;
	}	
	//printf("Stop motor...\n");
	_drv->stop();
	_drv->stopMotor();
	return true;
}

bool RPLidarController::startMotor(){
	if(!_drv){
		clean();
		return false;
	}
	//printf("Start motor...\n");
	_drv->startMotor();
	_drv->startScanExpress(0,_mode);
	return true;
}

void RPLidarController::reset(){
	//printf("Reset motor...\n");
	_drv->reset();
}

ScanData * RPLidarController::getScanArray(int countNodes, bool inverted, bool angleCompensate){
	rplidar_response_measurement_node_hq_t nodes[2048];
	size_t count = _countof(nodes);
	//int scan_duration;

	if (_data->intensities){ 
		//printf("Remove memory for intensities.\n");
		delete [] _data->intensities;
		_data->intensities = NULL;
	}
	if (_data->ranges){ 
		//printf("Remove memory for ranges.\n");
		delete [] _data->ranges;
		_data->ranges = NULL;
	}	

	u_result opResult;
	//time_t start_scan_time = time(NULL);
	//printf("Time start: %ld\n", start_scan_time);

	opResult = _drv->grabScanDataHq(nodes, count);

	//time_t end_scan_time = time(NULL);
	//printf("Time end: %ld\n", end_scan_time);
	//scan_duration = (end_scan_time - start_scan_time);
	//printf("Time interval: %ld\n",scan_duration);
	if (opResult == RESULT_OK) {
        opResult = _drv->ascendScanData(nodes, count);

        float angleMin = DEG2RAD(0.0f);	// = 0.0
        float angleMax = DEG2RAD(359.0f);	// = 2*pi
        if (opResult == RESULT_OK) {
            if (angleCompensate) {
			//printf("Using angle compensate.\n");
                int angleCompensateNodesCount = countNodes;
                float angleCompensateMultiple = countNodes/65535.0;
                int angleCompensateOffset = 0;
                rplidar_response_measurement_node_hq_t angleCompensateNodes[angleCompensateNodesCount];
                memset(angleCompensateNodes, 0, angleCompensateNodesCount*sizeof(rplidar_response_measurement_node_hq_t));
                int i = 0, j = 0;
                for( ; i < count; i++ ) {
		    		//Choose valid nodes	
                    if (nodes[i].dist_mm_q2 != 0) {
                    	// map value from 0->65535 to 0->countNodes
                    	int angleValue = (int)(nodes[i].angle_z_q14 * angleCompensateMultiple);
			 			// _u16 angle_tmp = 1 | (((nodes[i].angle_z_q14 * 90) >> 8) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT);	
						// float angle = (float)((angle_tmp >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f);
    					// int angleValue = (int)(angle * angleCompensateMultiple);
    					if ((angleValue - angleCompensateOffset) < 0) angleCompensateOffset = angleValue;

			 			// limit the range values in angle_compensate, each degree (0->360) represent a value
						for (j = 0; j < angleCompensateMultiple; j++) {
    						angleCompensateNodes[angleValue-angleCompensateOffset+j] = nodes[i];
							// printf("Angle compensate nodes: %d ",angleValue-angleCompensateOffset+j);
                        }

			    	// printf("Count: %d hq: %d angle_hq: %d distance: %f quality: %d \n", i, nodes[i].angle_z_q14, angleValue-angleCompensateOffset, nodes[i].dist_mm_q2/4.0f, nodes[i].quality);	
                    }
                }	
                _data->nodeCount = angleCompensateNodesCount;
                getScanData(angleCompensateNodes, 
                    	angleCompensateNodesCount,
						//scan_duration, 
						inverted, angleMin, angleMax);
            } else {
		    	//printf("Not using angle compensate.\n");
                int startNode = 0, endNode = 0;
                int i = 0;
                // find the first valid node and last valid node -> find the first valid angle and the last valid angle
                while (nodes[i++].dist_mm_q2 == 0);
                startNode = i-1;
                i = count - 1;
                while (nodes[i--].dist_mm_q2 == 0);
                endNode = i+1;
				_u16 angleStartTmp = 1 | (((nodes[startNode].angle_z_q14 * 90) >> 8) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT);	
				_u16 angleEndTmp = 1 | (((nodes[endNode].angle_z_q14 * 90) >> 8) << RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT);	
                angleMin = DEG2RAD((float)(angleStartTmp >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f);
                angleMax = DEG2RAD((float)(angleEndTmp >> RPLIDAR_RESP_MEASUREMENT_ANGLE_SHIFT)/64.0f);
				//printf("Angle min: %f Angle max: %f\n", angle_min, angle_max);
				_data->nodeCount = endNode-startNode+1;
                getScanData(&nodes[startNode], endNode-startNode+1,
                //scan_duration, 
                inverted, angleMin, angleMax);
            }
        } else if (opResult == RESULT_OPERATION_FAIL) {
            // All the data is invalid, just publish them
            float angleMin = DEG2RAD(0.0f);
            float angleMax = DEG2RAD(359.0f);
			//printf("Data invalid.\n");
			_data->nodeCount = count;
            getScanData(nodes, count,
                        //scan_duration, 
            			inverted, angleMin, angleMax);
        }
	}
	return _data;
}

void RPLidarController::getScanData(
	rplidar_response_measurement_node_hq_t * nodes,				
	size_t nodeCount, 
	//int scan_time,
	bool inverted,
	float angleMin, 
	float angleMax){

	static int scanCount = 0;
	scanCount++;
	
	//if angle_max > angle_min: reverse the data ranges <-> descending the data based on degree)
	bool reversed = (angleMax > angleMin);
	
	//force angle ranges (-pi,pi)
	if(reversed){
		_data->angleMin = M_PI - angleMax;
		_data->angleMax = M_PI - angleMin;
	} else {
		_data->angleMin = M_PI - angleMin;
		_data->angleMax = M_PI - angleMax;
	}
	_data->angleIncrement = (_data->angleMax - _data->angleMin)/(double)(nodeCount - 1);

	// _data->rangeMin = 0.15; // meters
	// _data->rangeMax = 6.0;

	_data->intensities = new float[nodeCount];
	_data->ranges = new float[nodeCount];

	bool reverseData = (!inverted && reversed) || (inverted && !reversed);
	
	if (!reverseData){
		for(size_t i = 0; i < nodeCount; i++){
			// float readValue = (float)nodes[i].dist_mm_q2/4.0f/1000;
			float readValue = (float)nodes[i].dist_mm_q2/4.0f/10;
			if(readValue == 0.0)
				_data->ranges[i] = std::numeric_limits<float>::infinity();
			else
				_data->ranges[i] = readValue;
			_data->intensities[i] = (float)nodes[i].quality;
		}
	} else {
		for (size_t i = 0; i < nodeCount; i++){
			// float readValue = (float)nodes[i].dist_mm_q2/4.0f/1000;
			float readValue = (float)nodes[i].dist_mm_q2/4.0f/10.0;
			if(readValue == 0.0)
				_data->ranges[nodeCount-1-i] = std::numeric_limits<float>::infinity();
			else
				_data->ranges[nodeCount-1-i] = readValue;
			_data->intensities[nodeCount-1-i] = (float)nodes[i].quality;
		}
	}
}

void RPLidarController::clean(){
	if (_data->intensities){ 
		//printf("Remove memory for intensities.\n");
		delete [] _data->intensities;
		_data->intensities = NULL;
	}
	if (_data->ranges){ 
		//printf("Remove memory for ranges.\n");
		delete [] _data->ranges;
		_data->ranges = NULL;
	}
	if(_data){
		//printf("Remove memory for data.\n");
		delete _data;
		_data = NULL;
	}
	if(_drv){
		//printf("Remove memory for driver.\n");
		delete _drv;
		_drv = NULL;
	}
}
