#include "GrowingBrushStokeAct.h"

GrowingBrushStokeAct::GrowingBrushStokeAct() :
transparency( 255 )
{
	setup();

}

GrowingBrushStokeAct::~GrowingBrushStokeAct()
{

}


void GrowingBrushStokeAct::setup() {

	//ofSetLogLevel( OF_LOG_ERROR );
	//ofSetVerticalSync( true );
	//ofSetFrameRate( 30 );

	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	background.loadImage( "lines_3_bw.jpg" );

	doJiggle = true;
	scaleNoiseVal = 0.0f;
	rotateNoiseVal = 0.0f;
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

	

	tintBuffer.allocate( 1920, 1080 );
	stoneCurtainBuffer.allocate( 1920, 1080 );

	addCustomVoronoiPoints();
}

void GrowingBrushStokeAct::createStone( ofPoint centerStone )
{
	plainStone.setColorCollection( blackWhiteColor );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	std::cout << "createstone: " << centerStone.x << " " << centerStone.y << std::endl;
	plainStone.init( centerStone.x, centerStone.y );
	for( int i = 0; i < 1; i++ ) {
		plainStone.growPlain( growBrushIndex );
	}
}

void GrowingBrushStokeAct::addCustomVoronoiPoints() {

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


void GrowingBrushStokeAct::update() {
	if( ofGetFrameNum() % 2 == 0 ) {
		plainStone.grow( *voro.getLine( 0 ) );
	}

	curtainX++;
}

void GrowingBrushStokeAct::updateJiggle()
{
	scaleNoiseVal += 0.05f;
	rotateNoiseVal += 0.01f;
}

void GrowingBrushStokeAct::draw() {

	//ofBackground( 0 );

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
		float scale = ( ofNoise( scaleNoiseVal ) + 0.5 );
		ofScale( scale, scale );
		ofRotateZ( ( ofNoise(rotateNoiseVal) * 2 - 1 ) * 20 );
	}

	plainStone.setSelectedColor( ofColor( 255 ) );
	plainStone.setTransparency( 127 );
	plainStone.draw( -1920 / 2, -1080/2, 1920, 1080);

	ofPopMatrix();
	
	ofPopStyle();
	edgeDetectionPostProcessing.end();
	tintBuffer.end();

	ofPushStyle();
	ofSetColor( 255, transparency );
	//ofSetColor( 255, 200, 0, 120 );
	tintBuffer.draw( 0, 0 );
	ofPopStyle();

	// drawing stone curtain
	// stoneCurtainBuffer.begin();
	// post.begin();
	// stoneCurtain.draw( 0, 0 );
	// post.end();
	// stoneCurtainBuffer.end();

	//ofPushStyle();
	//ofSetColor( 163, 87, 52, 120 );
	//stoneCurtainBuffer.draw( curtainX, 0 );
	//ofPopStyle();

	//voro.draw( 0, 0 );

	//slowWarp.end();
}

void GrowingBrushStokeAct::keyPressed( int key )
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

