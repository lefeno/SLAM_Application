#include "MeasurementModel.h"

//x_bg: position of beams in global coordinate
//y_bg: position of beams in global coordinate
//x_sl: position of sensor in robot fixed local coordinate
//y_sl: position of sensor in robot fixed local coordinate
//theta_bl: position of beams in robot fixed local coordinate
//occ: contain coordinates of occupied grids
float MeasurementModel::measurementModel(ScanData * data, Vector3f pose, vector<Vector2f> occ, Vector2f sensorPose){
	float q = 1;
	float dist2 = 0, tempDist2 = 0;
	float x_bg, y_bg, x_sl = sensorPose[0], y_sl = sensorPose[1], theta_bl;	//the sensor's position is the same as robot's position
	for(int i = 0; i < data->nodeCount; i++){
		if(data->ranges[i] < data->rangeMax - data->rangeMin){
			//calculate theta_bl
			theta_bl = (i + 1) * data->angleIncrement;
			if(theta_bl > M_PI){
				theta_bl -= 2*M_PI;
			}
			x_bg = pose[0] + x_sl * cos(pose[2]) - y_sl * sin(pose[2]) + data->ranges[i] * cos(pose[2] + theta_bl);
			y_bg = pose[1] + y_sl * cos(pose[2]) + x_sl * sin(pose[2]) + data->ranges[i] * sin(pose[2] + theta_bl);
			//calculate dist^2
			dist2 = (x_bg - occ[0][0])*(x_bg - occ[0][0]) + (y_bg - occ[0][1])*(y_bg - occ[0][1]);
			for(int j = 1; j < occ.size(); ++j){
				tempDist2 = (x_bg - occ[i][0])*(x_bg - occ[i][0]) + (y_bg - occ[i][1])*(y_bg - occ[i][1]);
				if(dist2 > tempDist2){
					dist2 = tempDist2;
				}
			}			
			q = q*(_zhit*General::prob(dist2,_sig_hit*_sig_hit) + _zrand/data->rangeMax);	//dist2 or sqrt(dist2)??
		}
	}
	return q;
}

Vector3f MeasurementModel::firstDerivativeLogRangeFinderModel(ScanData* data, Vector3f pose, vector<Vector2f> occ, Vector2f sensorPose){
	Vector3f d(0,0,0), d_dist2(0,0,0), d_b(0,0,0); //represent for dx, dy, dtheta respectively
	Vector3f log_q(0,0,0), k(sensorPose[0],sensorPose[1],0);	
	Vector2f ok(0,0), d_ok(0,0);
	
	float dist2, tempDist2, x_, y_, a, b, c;
	for(int i = 0; i < data->nodeCount - 1; ++i){
		if(data->ranges[i] < data->rangeMax - data->rangeMin){
			k[2] = (i + 1) * data->angleIncrement;
			if(k[2] > M_PI){
				k[2] -= 2*M_PI;
			}

			ok[0] = pose[0] + k[0]*cos(pose[2]) - k[1] * sin(pose[2]) + data->ranges[i]*cos(pose[2] + k[2]);
			ok[1] = pose[1] + k[1]*cos(pose[2]) + k[0] * sin(pose[2]) + data->ranges[i]*sin(pose[2] + k[2]);
			d_ok[0] = -k[0] * sin(pose[2]) - k[1] * cos(pose[2]) - data->ranges[i] * sin(pose[2] + k[2]);
			d_ok[1] = -k[1] * cos(pose[2]) + k[1] * cos(pose[2]) + data->ranges[i] * cos(pose[2] + k[2]);
			
			dist2 = (ok[0] - occ[0][0])*(ok[0] - occ[0][0]) + (ok[1] - occ[0][1])*(ok[1] - occ[0][1]);
			x_ = occ[0][0];
			y_ = occ[0][1];
			for(int j = 1; j < occ.size(); ++j){
				tempDist2 = (ok[0] - occ[i][0])*(ok[0] - occ[i][0]) + (ok[1] - occ[i][1])*(ok[1] - occ[i][1]);
				if(dist2 > tempDist2){
					dist2 = tempDist2;
					x_ = occ[i][0];
					y_ = occ[i][1];
				}
			}			

			// dist2 = (ok[0] - x_) * (ok[0] - x_) + (ok[1] - y_) * (ok[1] - y_);
			d_dist2[0] = 2*(ok[0] - x_);
			d_dist2[1] = 2*(ok[1] - y_);
			d_dist2[2] = 2 * ((ok[0] - x_)*d_ok[0] + (ok[1] - y_)*d_ok[1]);

			a = _zhit*1.0/sqrt(2*M_PI*_sig_hit*_sig_hit);
			b = -0.5*dist2/(_sig_hit*_sig_hit);	//dist2 or (dist2)^2??
			c = (1 - _zhit)/data->rangeMax;	// don't use zmax?

			d_b[0] = -0.5*d_dist2[0]/(_sig_hit * _sig_hit);
			d_b[1] = -0.5*d_dist2[1]/(_sig_hit * _sig_hit);
			d_b[2] = -0.5*d_dist2[2]/(_sig_hit * _sig_hit);

			// Should I calculate logq?? (line 20 page 413)
			log_q[0] = a*exp(b)*d_b[0]/(c + a*exp(b));
			log_q[1] = a*exp(b)*d_b[1]/(c + a*exp(b));
			log_q[2] = a*exp(b)*d_b[2]/(c + a*exp(b));

			d[0] += log_q[0];
			d[1] += log_q[1];
			d[2] += log_q[2];
		}
	}
	return d;
}

