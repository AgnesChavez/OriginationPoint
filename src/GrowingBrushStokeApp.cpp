#include "GrowingBrushStokeApp.h"

void GrowingBrushStokeApp::setup() {

	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	background.loadImage( "lines_3_bw.jpg" );

	doJiggle = true;
	noiseVal = 0.0f;
	growBrushIndex = 0;
	curtainX = -1920;
	
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

	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( agnesColorSelection );

	//stoneCurtain.render();

	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	plainStone.setColorCollection( blackWhiteColor );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	plainStone.init( 1920 / 2, 1080 / 2 );
	for( int i = 0; i < 3; i++ ) {
		plainStone.growPlain( growBrushIndex );
	}

	tintBuffer.allocate( 1920, 1080 );
	stoneCurtainBuffer.allocate( 1920, 1080 );

	voro.clear();

	voro.addPoint( 1920 / 2, 1080 / 2 );
	voro.addPoint( ofRandom( 100, 500 ), 100 );
	voro.addPoint( ofRandom( 500, 1500 ), 100 );
	voro.addPoint( ofRandom( 1500, 1900 ), 100 );

	voro.addPoint( ofRandom( 100, 500 ), 1000 );
	voro.addPoint( ofRandom( 500, 1000 ), 1000 );
	voro.addPoint( ofRandom( 1000, 1900 ), 1000 );

	voro.addPoint( 100, ofRandom( 100, 300 ) );
	voro.addPoint( 100, ofRandom( 300, 800 ) );
	voro.addPoint( 100, ofRandom( 800, 1100 ) );

	voro.addPoint( 1800, ofRandom( 100, 300 ) );
	voro.addPoint( 1800, ofRandom( 300, 800 ) );
	voro.addPoint( 1800, ofRandom( 800, 1100 ) );

	voro.setSmoothAmount( 15 );

	voro.compute();
	voro.render();
}


void GrowingBrushStokeApp::update() {

	if( ofGetFrameNum() % 2 == 0 ) {
		plainStone.grow( *voro.getLine( 0 ) );
	}

	noiseVal += 0.01f;

	curtainX++;
}

void GrowingBrushStokeApp::draw() {

	ofBackground( 0 );

	//postWarp.begin();
	//background.draw( 0, 0, 1920, 1080 );
	//postWarp.end();
	//float x = ofMap( ofGetMouseX(), 0, 1920, -1080, 1080 );

	/*
	ofPushStyle();
	ofSetColor( 255, 255, 255, ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	waterColorStone.draw( - x, 0 );
	ofPopStyle();
	*/
	//slowWarp.begin();

	tintBuffer.begin();
	ofClear( 0, 0, 0, 0 );
	edgeDetectionPostProcessing.begin();
	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	if( doJiggle ) {
		ofScale( ofNoise( noiseVal ) + 0.5, ofNoise( noiseVal ) + 0.5);
		ofRotateZ( ( ofNoise(noiseVal) * 2 - 1 ) * 20 );
	}

	plainStone.setSelectedColor( ofColor( 255 ) );
	plainStone.setTransparency( 127 );
	plainStone.draw( -1920 / 2, -1080/2, 1920, 1080);

	ofPopMatrix();
	
	ofPopStyle();
	edgeDetectionPostProcessing.end();
	tintBuffer.end();

	ofPushStyle();
	ofSetColor( 255, 200, 0, 120 );
	tintBuffer.draw( 0, 0 );
	ofPopStyle();

	// drawing stone curtain
	// stoneCurtainBuffer.begin();
	// post.begin();
	// stoneCurtain.draw( 0, 0 );
	// post.end();
	// stoneCurtainBuffer.end();

	ofPushStyle();
	ofSetColor( 163, 87, 52, 120 );
	stoneCurtainBuffer.draw( curtainX, 0 );
	ofPopStyle();

	//voro.draw( 0, 0 );

	//slowWarp.end();
}

void GrowingBrushStokeApp::keyPressed( int key )
{
	if( key == 'g' ) {
		// 0 means random brush
		//plainStone.growPlain( growBrushIndex );
		plainStone.grow( *voro.getLine( 0 ) );
	}
	else if( key == 'f' ) {
		ofToggleFullscreen();
	}
	else if( key == 'j' ) {
		doJiggle = !doJiggle;
	}
	else if( key == 'q' ) {
		growBrushIndex++;
	}
	else if( key == 'w' ) {
		growBrushIndex--;
	}
	else if( key == 'i' ) {
		
	}
}
