#pragma once

#include "ofMain.h"

#include "StopMotionStonesAct.h"
#include "GrowingBrushStokeAct.h"
#include "StoneCurtainLayer.h"
#include "KinectInteractionManager.h"

#include "ofxPlaylist.h"
#include "ofxQuadWarp.h"
#include "ofxTimeMeasurements.h"

#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 5001

class ActSequencer : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	// timing
	unsigned long long currentMillisTimelinePosition, lastElapsedMillis;
	
	// kinect input
	KinectInteractionManager kinect;

	// osc output
	ofxOscSender sender;

	// projection mapping
	ofxQuadWarp warper;

	StopMotionStonesAct * act1;
	GrowingBrushStokeAct * act2;
	StoneCurtainLayer * act3;

	static std::vector< ofPoint > getLineSplitPoints( ofPolyline linesToSplit, float length );
	static void drawSplitLines( std::vector< ofPoint > points );

private:
	void sendOscMessages( KinectInteractionManager * kinect);
};

