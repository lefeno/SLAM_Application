/*
 * MotionModel.h
 *
 *  Created on: May 04, 2019
 *      Author: linhtht
 */

#include "MotionModel.h"

Vector3f MotionModel::getOdomData(Vector3f odPose_t, Vector3f odPose_t1){
	Vector3f odom;	//(rot1, trans, rot2)
	odom(0) = atan2(odPose_t(1) - odPose_t1(1),odPose_t(0) - odPose_t1(0)) - odPose_t1(2);
	odom(1) = sqrt((odPose_t1(0) - odPose_t(0))*(odPose_t1(0) - odPose_t(0)) + (odPose_t1(1) - odPose_t(1))*(odPose_t1(1) - odPose_t(1)));
	odom(2) = odPose_t(2) - odPose_t1(2) - odom(0);
	return odom;
}

Vector3f MotionModel::sampleMotionModelOdometry(Vector3f odPose_t, Vector3f odPose_t1, Vector3f pose_t1){
	Vector3f odom = getOdomData(odPose_t, odPose_t1);
	float predictRot1, predictTrans, predictRot2;
	predictRot1 = odom(0) - sample(_a1*odom(0) + _a2*odom(1));
	predictTrans = odom(1) - sample(_a3*odom(1) + _a4*(odom(0) + odom(2)));
	predictRot2 = odom(2) - sample(_a1*odom(2) + _a2*odom(1));	
	
	Vector3f poseGuess;
	poseGuess(0) = pose_t1(0) + predictTrans*cos(pose_t1(2)+predictRot1);
	poseGuess(1) = pose_t1(1) + predictTrans*sin(pose_t1(2)+predictRot1);
	poseGuess(2) = pose_t1(2) + predictRot1 + predictRot2;

	return poseGuess;
}

float MotionModel::sample(float a){
	if(_useNormDist){
		return sampleNormalDistribution(a);
	}
	return sampleTriangularDistribution(a);
}

float MotionModel::sampleNormalDistribution(float b){
	float temp = 0;
	for(int i = 0; i<12; ++i){
		temp += General::genRandNumber(-1,1);
	}
	return b*temp/6.0;
}

float MotionModel::sampleTriangularDistribution(float b){
	return b*General::genRandNumber(-1,1)*General::genRandNumber(-1,1);
}

//Need to check the model more
Vector3f MotionModel::firstDerivativeLogMotionModelOdometry(Vector3f poseN, Vector3f odPose_t, Vector3f odPose_t1, Vector3f pose){
	Vector3f odom = getOdomData(odPose_t,odPose_t1);
	Vector3f realMotion = getOdomData(poseN, pose);

	// Add more cases ??
	float dRot1xN = (pose(1) - poseN(1))/((poseN(0) - pose(0))*(poseN(0) - pose(0)) + (poseN(1) - pose(1))*(poseN(1) - pose(1)));
	float dRot1yN = (poseN(0) - pose(0))*(poseN(0) - pose(0))/((poseN(0) - pose(0))*(poseN(0) - pose(0)) + (poseN(1) - pose(1))*(poseN(1) - pose(1)));
	float dTransxN = (poseN(0) - pose(0))/realMotion(1);
	float dTransyN = (poseN(1) - pose(1))/realMotion(1);

	Vector3f arg1, arg2;
	arg1(0) = odom(0) - realMotion(0); arg2(0) = _a1*realMotion(0) + _a2*realMotion(1);
	arg1(1) = odom(1) - realMotion(1); arg2(1) = _a3*realMotion(1) + _a4*(realMotion(0) + realMotion(2));
	arg1(2) = odom(2) - realMotion(2); arg2(2) = _a1*realMotion(2) + _a2*realMotion(1);

	Vector3f darg1xN, darg2xN, darg1yN, darg2yN, darg1thetaN, darg2thetaN;
	darg1xN(0) = -dRot1xN; 						darg1yN(0) = -dRot1yN; 						darg1thetaN(0) = 0;
	darg2xN(0) = _a1*dRot1xN + _a2*dTransxN;	darg2yN(0) = _a1*dRot1yN + _a2*dTransyN; 	darg1thetaN(0) = 0;
	darg1xN(1) = -dTransxN; 					darg1yN(1) = -dTransyN; 					darg1thetaN(1) = 0;
	//Check this again??
	darg2xN(1) = _a3*dTransxN + _a4*dRot1xN;	darg2yN(1) = _a3*dTransyN + _a4*dRot1yN; 	darg1thetaN(1) = _a4;
	darg1xN(2) = 0; 							darg1yN(2) = 0; 							darg1thetaN(2) = -1;
	darg2xN(2) = _a2*dTransxN;					darg2yN(2) = _a2*dRot1yN; 					darg1thetaN(2) = _a1;

	float pi = 0, p = 0;
	Vector3f dpN(0,0,0);
	Vector3f dpiN(0,0,0);
	Vector2f dprob(0,0);
	for(int i = 0; i < 2; ++i){
		pi = General::prob(arg1(i),arg2(i),_useNormDist);
		dprob = firstDerivativeProbNormalDistribution(arg1(i), arg2(i));
		dpiN(0) = dprob(0)*darg1xN(i) + dprob(1)*darg2xN(i);
		dpiN(1) = dprob(0)*darg1yN(i) + dprob(1)*darg2yN(i);
		dpiN(2) = dprob(0)*darg1thetaN(i) + dprob(1)*darg2thetaN(i);
		// p += log(pi);
		dpN(0) += (1.0/pi)*dpiN(0);
		dpN(1) += (1.0/pi)*dpiN(1);
		dpN(2) += (1.0/pi)*dpiN(2);
	}
	return dpN;
}

Vector2f MotionModel::firstDerivativeProbNormalDistribution(float d, float b){
	float prob = 1.0/sqrt(2*M_PI*b)*exp(-0.5*d*d/b);
	Vector2f dprob;
	dprob(0) = -prob*d/b;
	dprob(1) = (d*d/(2*b*b) - 1/(2*b))*prob;

	return dprob;
}