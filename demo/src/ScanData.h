
#ifndef SCANDATA_H__
#define SCANDATA_H__

#include <Eigen/Dense>

class ScanData{
public:
	float rangeMin;
	float rangeMax;
	float angleMin;
	float angleMax;
	float angleIncrement;
	float* ranges;
	float* intensities;
	int nodeCount;
	//float scanTime;
	
	ScanData(){
	}
	
	~ScanData(){
	}
	
	ScanData(float rangeMin, float rangeMax, float angleMin, float angleMax, float angleIncrement, float* ranges, float* intensities, int nodeCount){
		this->rangeMin = rangeMin;
		this->rangeMax = rangeMax;
		this->angleMin = angleMin;
		this->angleMax = angleMax;
		this->angleIncrement = angleIncrement;
		this->ranges = ranges;
		this->intensities = intensities;
		this->nodeCount = nodeCount;
		//scanTime = scanTime;
	}	
};

#endif