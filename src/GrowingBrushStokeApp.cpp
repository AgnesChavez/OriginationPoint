#include "GrowingBrushStokeApp.h"

void GrowingBrushStokeApp::setup() {

	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	
	ofBackground( 0 );
	stone8ColorCollection.addColor( 236, 73, 78 );
	stone8ColorCollection.addColor( 197, 153, 72 );
	stone8ColorCollection.addColor( 211, 182, 60 );
	stone8ColorCollection.addColor( 197, 190, 51 );
	stone8ColorCollection.addColor( 202, 120, 78 );
	stone8ColorCollection.addColor( 152, 103, 100 );

	brightYellowColorCollection.addColor( 195, 189, 75 );
	brightYellowColorCollection.addColor( 150, 144, 70 );
	brightYellowColorCollection.addColor( 73, 78, 46 );
	brightYellowColorCollection.addColor( 201, 198, 85 );
	brightYellowColorCollection.addColor( 209, 202, 95 );

	brownColorCollection.addColor( 102, 51, 0 );
	brownColorCollection.addColor( 153, 102, 51 );
	brownColorCollection.addColor( 187, 153, 102 );

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );


	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	plainStone.setColorCollection( blackWhiteColor );
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

	//ofBackground( 0 );
	ofPushStyle();
	ofSetColor( 255, 255, 255, ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	waterColorStone.draw( - x, 0 );
	ofPopStyle();

	plainStone.setTransparency( 255 - ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	plainStone.draw( x, 0, 1920, 1080 );
}

void GrowingBrushStokeApp::keyPressed( int key )
{
	ofToggleFullscreen();
}
