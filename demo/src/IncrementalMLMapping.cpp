/*
 * RPLidarController.h
 *
 *  Created on: May 05, 2019
 *      Author: linhtht
 */
#include "IncrementalMLMapping.h"

void IncrementalMLMapping::augmented_MCL(Vector3f xt, Vector3f xt1, ScanData* zt, OccuGridMap * m){
	int size = _Xt1.size();
	float wAvg = 0, w = 0;
	vector<Vector3f> Xt_;
	_Xt.clear(); 
	vector<float> wt_;
	Vector3f randPose;
	Vector3f temp;
	float sum = 0;
	for(int n = 0; n < size; ++n){
		temp = _motion->sampleMotionModelOdometry(xt, xt1, _Xt1[n]);
		w = _measurement->measurementModel(zt, temp, m->getOccGrids());
		Xt_.push_back(temp);
		wt_.push_back(w);
		wAvg += 1.0/size*w;
		sum += w;
	}
	_wSlow = _wSlow + _aSlow*(wAvg - _wSlow);
	_wFast = _wFast + _aFast*(wAvg - _wFast);
	//if sum(wt_) != 1 -> need to normalize it ????????
	for(int n = 0; n < size; ++n){
		if(General::genRandNumber(0,1) < (1.0 - _wFast/_wSlow)){
			//add random pose to Xt
			randPose = generateRandPose(m);
			_Xt.push_back(randPose);
		} else {
			// draw i from (0,size), using resampling wheel
			int index = rand() % size;
			float beta = 0.0;
			int mw = std::max_element(wt_.begin(),wt_.end()) - wt_.begin();
			
			beta += General::genRandNumber(0,2.0*mw);
			while(beta > (wt_[index]/sum)){
				beta -= (wt_[index]/sum);
				index = (index + 1) % size;
			}
			_Xt.push_back(Xt_[n]);
		}
	}
}

Vector3f IncrementalMLMapping::generateRandPose(OccuGridMap * m){
	Vector3f randPose;
	if(General::genRandNumber(-1,1) > 0){
		randPose[2] = General::genRandNumber(0,M_PI);	
	} else {
		randPose[2] = General::genRandNumber(0,M_PI) * -1 ;
	}
	
	randPose[1] = General::genRandNumber(0, m->getMapSizeY());
	randPose[0] = General::genRandNumber(0, m->getMapSizeX());
	return randPose;
}

Vector3f IncrementalMLMapping::getPossiblePose(){
	int max = 0, co = 0;
	Vector3f most = _Xt[0];
	for(int i = 0; i < _Xt.size(); ++i){
		co = (int)count(_Xt.begin(), _Xt.end(), _Xt[i]);
		if(co > max){
			max = co;
			most = _Xt[i];
		}
	}
	return most;
}

void IncrementalMLMapping::createRandXt(OccuGridMap * m){
	_Xt.clear();
	for(int i = 0; i < _sizeXt; ++i){
		_Xt.push_back(generateRandPose(m));
	}
}

Vector3f IncrementalMLMapping::incrementalMLMapping(ScanData* data, Vector3f pose, Vector3f odPose_t, Vector3f odPose_t1, OccuGridMap * m){
	//Need to find this value
	int k = 0.2;
	augmented_MCL(odPose_t, odPose_t1, data, m);
	// set s_ = argmaxP(s_|a,s)
	Vector3f s_ = getPossiblePose();
	Vector3f evaluation = Vector3f(1,1,1), s_pre = s_;
	int i = 0;
	while(i < 10000 || abs(evaluation.sum()) < 0.01){
		s_ = s_ + k * (_measurement->firstDerivativeLogRangeFinderModel(data, s_, m->getOccGrids()) + _motion->firstDerivativeLogMotionModelOdometry(s_, odPose_t, odPose_t1, pose));
		evaluation = (s_pre - s_).cwiseAbs();
		s_pre = s_;
		++i;
	}

	m->updateMap(s_,data);
	return s_;
}

