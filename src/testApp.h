#pragma once

#include "ofMain.h"
//#include "WaterColorCanvas.h"
#include "Stone.h"
#include "VoronoiLayer.h"

#include "ofxPSBlend.h"
#include "ofxUI.h"
#include "ofxTimeMeasurements.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void clearLayers();

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

	ofImage bg;
	void reinit();
	ofFbo buffer;
	bool doGrow;

	BrushCollection brushCollection;
	ColorCollection stone8ColorCollection;


	ofxUISuperCanvas *gui;
	void setupGui();
	void guiEvent( ofxUIEventArgs &e );

	// blending
	ofxPSBlend psBlend;
	int blendMode;

	
};