#pragma once

#include "ofMain.h"
#include "WaterColorCanvas.h"
#include "Stone.h"

#include "ofxPSBlend.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void strokeDraw();
	void clearLayers();
	void drawRockOnSurface();
	void drawWater();
	void drawStoneUnderlyingShape();
	void drawRockOnSurfaceMoreColor(float x, float y );

	Stone stone;

	ofImage getRandomBrush();
	ofFbo rockShape;
	ofPixels rockPixels, randomNoisePixels;
	ofFbo randomNoiseBuffer;
	bool drawFbo, applyPointillistic;
	int randomBrushId;
	int brushIndex;

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
	ofImage brush, specialBrush;
	std::vector< ofImage > brushes;

	WaterColorCanvas canvas;

	ofPoint pos, prev, vec;
	float width, currentWidth;
	int currentPigment;
	bool pressed;

	int state;
	static const int STATE_WATER = 0;
	static const int STATE_PIGMENT = 1;
	static const int STATE_MIX = 2;

	ofxUISuperCanvas *gui;
	void setupGui();
	void guiEvent( ofxUIEventArgs &e );

	// blending
	ofxPSBlend psBlend;
	int blendMode;

	void drawIndependently();
	ofFbo buffer;
};