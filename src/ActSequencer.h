#pragma once

#include "ofMain.h"

#include "StopMotionStonesAct.h"
#include "GrowingBrushStokeAct.h"
#include "StoneCurtainLayer.h"
#include "KinectInteractionManager.h"

#include "ofxPlaylist.h"
#include "ofxQuadWarp.h"
#include "ofxTimeMeasurements.h"
#include "ofxUI.h"
#include "ofxOsc.h"

#include "ofxAnimatableFloat.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxAnimatableOfColor.h"

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
	bool hasSentAct1, hasSentAct2, hasSentAct3;
	bool hasSentPrevAct2, hasSentPrevAct3;

	// projection mapping
	ofxQuadWarp warper;

	// the three acts
	StopMotionStonesAct * act1;
	GrowingBrushStokeAct * act2;
	StoneCurtainLayer * act3;

	// gui
	ofxUISuperCanvas *gui;
	void setupGui();
	void guiEvent( ofxUIEventArgs &e );

	// general offscreen buffer
	ofFbo buffer;

	bool visualTrigger, prevVisualTrigger;

	ofxAnimatableOfColor bigRockColor, fourRocksColor, curtainLeftColor, curtainRightColor;

	ofColor bigRockColorGui, fourRockColorGui, curtainLeftColorGui, curtainRightColorGui;

private:
	void sendKinectOscMessages( KinectInteractionManager * kinect );
	void sendChapterOscMessages( int chapterId );
	void sendPreChapterOscMessages( int chapterId );
};

