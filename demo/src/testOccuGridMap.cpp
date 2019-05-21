#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <app/inc/rplidar.h> //RPLIDAR standard sdk, all-in-one header
#include "RPLidarController.h"
#include <math.h>
#include <sys/time.h>
#include <Eigen/Dense>	//Library for matrix and vector

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace Eigen;
using namespace std;
using namespace cv;

int main(){

	RPLidarController rplidar;
	rplidar.initialize("/dev/ttyUSB0",115200,1);
	//rplidar.stopMotor();
	rplidar.startMotor();
	ScanData * data = NULL;


// Build an occupancy grid map
	int mapSize = 600;

	Mat1f image(mapSize,mapSize);

	float mapResolution = 5;	//cm
	int mapSizeX = mapSize;
	int mapSizeY = mapSize;
	float totalMapSizeX = mapSizeX * mapResolution;
	float totalMapSizeY = mapSizeY * mapResolution;
	float x0 = 0.5 * mapSizeX;
	float y0 = 0.5 * mapSizeY;

	float l_occ = 10;
	float l_free = 100;
	float p_i1 = 0.5;
	float l0 = log(p_i1/(1-p_i1)) + 255;
	Vector2f initialPose(x0,y0);
	MatrixXd map = MatrixXd::Constant(mapSize, mapSize, l0);
	double r = 0;
	// VectorXf grid(2);

	float laserRangeMax = 300;	//cm
	float alpha = 1;
	// each beam will have a dist of 5cm
	float beta = (1*M_PI)/180.0; //degree to rad between each beam, map 0 -> 360 to 0 -> 720, use boost mode each scan 1064 points
	// 
	// cout << "Initial pose = " << initialPose(0) << " " << initialPose(1) << endl;
	// cout << "Map = " << endl << map << endl;
	// cout << "Grid = " << endl << grid << endl;

	// while(1){
		data = rplidar.getScanArray();

	// //occupancy_grid_mapping
		float xi = 0, yi = 0, theta = 0;
		for(int i = 0; i < mapSize; ++i){
			for(int j = 0; j < mapSize; ++j){
				// xi = i*mapResolution - mapResolution/2;
				// yi = j*mapResolution - mapResolution/2;
				xi = i;
				yi = j;
				r = sqrt((xi - x0)*(xi - x0) + (yi - y0)*(yi - y0));
	// 			//r /= 100;
	// 			//printf("grid(%d, %d): %f\n", i, j, r);
				if ( r > laserRangeMax ){
					// continue;
				} else {
	// 				//printf("grid(%d, %d): \n", i, j);
					float temp = 0.0;
					float phi = atan2(yi-y0,xi-x0) - theta; //what is this theta: robot direction
	// 				//find k from neighbor beams
					int k = phi/beta, k0 = 0, k1 = 0;
					float k_angle = 0.0;
					if(k >= 0){
						k0 = k;
						k1 = k + 1;
					} else {
						k0 = k;
						k1 = k - 1;
					}
					// printf("i: %d,j:%d ,Phi: %f k: %d goc k: %f k+1: %f ",i, j, phi, k, (k0*beta), (k1)*beta);
					if(abs((k0*beta) - phi) > abs(((k1)*beta) - phi)){
						k = k1;
					} else {
						k = k0;
					}

					k_angle = k * beta;
					if(k < 0){
						k = 360 + k;
					}
					// printf("Get k: %d \n", k);
					if ((r > min((float)laserRangeMax, data->ranges[k] + alpha/2)) || (abs(phi - k_angle) > beta/2)){
						temp = l0;
					}
	
					else if (data->ranges[k] < laserRangeMax && abs(r - data->ranges[k]) < alpha/2){
						temp = l_occ;
					}
	
					else if (r <= data->ranges[k]){
						temp = l_free;
					}				

					map(i,j) += temp - l0;
					// printf("Range: %f data range[k]: %f LogOdds: %f Map(%d,%d): %f \n", r, data->ranges[k], temp, i, j, map(i,j));
				}
				image(i,j) = map(i,j);
			}
		}
		// cout << "Map = " << endl << map << endl;
	// }
	// for(int i = 0; i < 2; ++i){
	// 	data = rplidar.getScanArray(720);
	// 	printf("Infor: \n - Range min: %f \n - Range max: %f \n - Angle min: %f \n - Angle max: %f \n - Angle increment: %f\n", data->rangeMin, data->rangeMax, data->angleMin, data->angleMax, data->angleIncrement);
	// 	for(int pos = 0; pos < 720; ++pos){
	// 		printf("Pos: %d, Distance: %f, Quality: %f \n", pos, data->ranges[pos], data->intensities[pos]);
	// 	}
	// }
	
	rplidar.stopMotor();
	rplidar.clean();
	
	// data is deleted in clean() already, so just need to assign NULL
	data = NULL;
	
	// test eigen
	// MatrixXd m = MatrixXd::Constant(3,3,1.2);
 //  	//m = (m + MatrixXd::Constant(3,3,1.2)) * 50;
 //  	cout << "m =" << endl << m << endl;
	// VectorXd v(3);
 //  	v << 1, 2, 3;
 //  	cout << "m * v =" << endl << m * v << endl;
	image(x0,y0) = 0;
	image(x0+1,y0) = 0;
	image(x0,y0+1) = 0;
	image(x0-1,y0) = 0;
	image(x0,y0-1) = 0;
	// SAVE image
	imwrite("result.jpg",image);

	return 0;
}
