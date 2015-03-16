#pragma once

#include "ofMain.h"
#include "Stone.h"
#include "VoronoiLayer.h"

#include "ofxUI.h"
#include "ofxTimeMeasurements.h"
#include "ofxQuadWarp.h"

#include "KinectWrapper.h"
#include "StoneCurtain.h"
#include "BarbWire.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();

	std::vector< Stone > stones;
	VoronoiLayer voro;
	float points;

	void keyPressed( int key );
	void keyReleased( int key );
	void mouseMoved( int x, int y );
	void mouseDragged( int x, int y, int button );
	void mousePressed( int x, int y, int button );
	void mouseReleased( int x, int y, int button );
	void windowResized( int w, int h );
	void dragEvent( ofDragInfo dragInfo );
	void gotMessage( ofMessage msg );
	void exit();

	KinectWrapper wrapper;
	bool displayKinect;
	float kinectToStoneDistance;

	ofxQuadWarp warper;

	ofImage bg;
	void reinit();
	bool doGrow;

	BrushCollection brushCollection;
	ColorCollection stone8ColorCollection, brightYellowColorCollection, brownColorCollection;

	StoneCurtain stoneCurtain;
	int currentCurtainY;

	BarbWire barbWire;

	ofxUISuperCanvas *gui;
	void setupGui();
	void guiEvent( ofxUIEventArgs &e );
};