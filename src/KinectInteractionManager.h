#pragma once

#include "ofMain.h"
#include "KinectWrapper.h"

class KinectInteractionManager
{
public:
	KinectInteractionManager();
	~KinectInteractionManager();

	void init();
	void update();
	void draw();

	float kinectToStoneDistance;
	bool displayKinect;

private: 
	KinectWrapper wrapper;
	
	ofShader kinectShader;
	ofFbo kinectFbo, kinectFbo2;

};

