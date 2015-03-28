#include "GrowingBrushStokeApp.h"

void GrowingBrushStokeApp::setup() {

	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );

	post.init( 1920, 1080 );

	postWarp.init( 1920, 1080 );
	noiseWarp = postWarp.createPass<NoiseWarpPass>();
	noiseWarp->setEnabled( true );

	edgePass = post.createPass< EdgePass >();
	edgePass->setEnabled( true );

	slowWarp.init( 1920, 1080 );
	slowWarpPass = slowWarp.createPass<NoiseWarpPass>();
	slowWarpPass->setEnabled( true );
	//noiseWarp = post.createPass<NoiseWarpPass>();
	//noiseWarp->setEnabled( false );

	convPass = post.createPass<ConvolutionPass>();
	convPass->setEnabled( false );

	post.setFlip( false );

	background.loadImage( "lines_3_bw.jpg" );

	doJiggle = false;
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

	stoneCurtain.render();


	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	plainStone.setColorCollection( blackWhiteColor );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	plainStone.init( 1920 / 2, 1080 / 2, line );

	tintBuffer.allocate( 1920, 1080 );
	stoneCurtainBuffer.allocate( 1920, 1080 );
}


void GrowingBrushStokeApp::update() {

	noiseVal += 0.01f;

	curtainX++;

	if( ofGetFrameNum() % 5 == 0 ) {
		//waterColorStone.growAll();
		//plainStone.growPlain();
	}

	noiseWarp->setAmplitude( ofMap( ofGetMouseX(), 0, 1920, 0, 1 ) );
	noiseWarp->setFrequency( ofMap( ofGetMouseY(), 0, 1080, 0, 1 ) );

}

void GrowingBrushStokeApp::draw() {

	//ofBackground( 0 );
	postWarp.begin();
	background.draw( 0, 0, 1920, 1080 );
	postWarp.end();
	//float x = ofMap( ofGetMouseX(), 0, 1920, -1080, 1080 );

	/*
	ofPushStyle();
	ofSetColor( 255, 255, 255, ofMap( ofGetMouseY(), 0, 1080, 0, 255 ) );
	waterColorStone.draw( - x, 0 );
	ofPopStyle();
	*/
	slowWarp.begin();

	tintBuffer.begin();
	ofClear( 0, 0, 0, 0 );
	post.begin();
	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	if( doJiggle ) {
		ofScale( ofNoise( noiseVal ), ofNoise( noiseVal ));
		ofRotateZ( ofNoise(noiseVal) * 50 );
	}

	plainStone.setSelectedColor( ofColor( 255 ) );
	plainStone.setTransparency( 127 );
	plainStone.draw( -1920 / 2, -1080/2, 1920, 1080);

	ofPopMatrix();
	
	ofPopStyle();
	post.end();
	tintBuffer.end();

	ofPushStyle();
	ofSetColor( 255, 200, 0, 120 );
	tintBuffer.draw( -100, 0 );
	ofPopStyle();

	stoneCurtainBuffer.begin();
	post.begin();
	stoneCurtain.draw( 0, 0 );
	post.end();
	stoneCurtainBuffer.end();

	ofPushStyle();
	ofSetColor( 163, 87, 52, 120 );
	stoneCurtainBuffer.draw( curtainX, 0 );
	ofPopStyle();

	slowWarp.end();
}

void GrowingBrushStokeApp::keyPressed( int key )
{
	if( key == 'g' ) {
		// 0 means random brush
		plainStone.growPlain( growBrushIndex );
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
}
