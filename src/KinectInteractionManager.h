#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"

#include "KinectWrapper.h"

//#define KINECT_NOT_CONNCETED

class KinectInteractionManager
{
public:
	KinectInteractionManager();
	~KinectInteractionManager();

	void init();
	void update();
	void draw();

	std::vector< ofxCvBlob > getBlobs();

	bool doBlobDetection;

	float kinectToStoneDistance, offset;
	bool displayKinect;

private: 
	KinectWrapper wrapper;
	
	ofShader kinectShader;
	ofFbo kinectFbo, kinectFbo2;

	ofxCvGrayscaleImage gr;
	ofxCvContourFinder contourFinder;
	ofxCvColorImage colorImage;
	ofPixels pix;

	void drawWithNormals( const ofPolyline & polyline );
};

