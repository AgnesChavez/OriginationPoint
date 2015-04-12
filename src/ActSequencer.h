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

	// the three acts
	StopMotionStonesAct * act1;
	GrowingBrushStokeAct * act2;
	StoneCurtainLayer * act3;

private:
	void sendKinectOscMessages( KinectInteractionManager * kinect );
	void sendChapterOscMessages( int chapterId );
};

