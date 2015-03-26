#include "GrowingBrushStokeApp.h"

void GrowingBrushStokeApp::setup() {

	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	
	ofBackground( 0 );
	agnesColorSelection.addColor( 232, 151, 44 );
	agnesColorSelection.addColor( 84, 18, 0 );
	agnesColorSelection.addColor( 152, 194, 45 );
	agnesColorSelection.addColor( 43, 74, 12 );
	agnesColorSelection.addColor( 46, 31, 1 );
	agnesColorSelection.addColor( 237, 69, 69 );
	agnesColorSelection.addColor( 128, 51, 0 );
	agnesColorSelection.addColor( 110, 18, 0 );
	agnesColorSelection.addColor( 163, 87, 52 );
	agnesColorSelection.addColor( 71, 209, 108 );

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );


	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	plainStone.setColorCollection( agnesColorSelection );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	plainStone.init( 1920 / 4 * 3, 1080 / 2, line );

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;
	brushStoneBuffer.allocate( settings );
}


void GrowingBrushStokeApp::update() {
	if( ofGetFrameNum() % 5 == 0 ) {
		waterColorStone.growAll();
		plainStone.growPlain();
	}
}

void GrowingBrushStokeApp::draw() {

	float x = ofMap( ofGetMouseX(), 0, 1920, -1080, 1080 );

	ofPushStyle();
	ofSetColor( 255, ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	waterColorStone.draw( - x, 0 );
	ofPopStyle();

	ofPushStyle();
	plainStone.setSelectedColor( ofColor( 255, 255, 255 ) );
	plainStone.setTransparency( 255 - ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	plainStone.draw( x, 0, 1920, 1080 );
	ofPopStyle();
}

void GrowingBrushStokeApp::keyPressed( int key )
{
	ofToggleFullscreen();
}
