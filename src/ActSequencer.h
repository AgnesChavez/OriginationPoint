#pragma once

#include "ofMain.h"

#include "StopMotionStonesAct.h"
#include "GrowingBrushStokeAct.h"
#include "ManyLayersAct.h"
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

	KinectInteractionManager kinect;
	ofxOscSender sender;

	ofxQuadWarp warper;

	ofxPlaylist playlist;
	float testVal;

	StopMotionStonesAct * act1;

	GrowingBrushStokeAct * act2;
	float act2Transparency;
	int act2Ypos;
	int stoneCurtainXpos;

	unsigned long long currentMillisTimelinePosition, lastElapsedMillis;

	ManyLayersAct * act3;

	static std::vector< ofPoint > getLineSplitPoints( ofPolyline linesToSplit, float length );
	static void drawSplitLines( std::vector< ofPoint > points );
};

