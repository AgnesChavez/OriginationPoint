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
	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	background.loadImage( "lines_3_bw.jpg" );

	doJiggle = true;
	scaleNoiseVal = 0.0f;
	rotateNoiseVal = 0.0f;
	growBrushIndex = 0;
	
	ofBackground( 0 );

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	tintBuffer.allocate( settings );
	

	addCustomVoronoiPoints();
}

void GrowingBrushStokeAct::createStone( ofPoint centerStone )
{
	plainStone.setColorCollection( blackWhiteColor );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	plainStone.init( centerStone.x, centerStone.y );
	for( int i = 0; i < 3; i++ ) {
		plainStone.growPlain( growBrushIndex );
	}
}

void GrowingBrushStokeAct::addCustomVoronoiPoints() {

	voro.clear();

	voro.addPoint( 1920 / 2, 1080 / 2 );
	voro.addPoint( ofRandom( 100, 300 ), 100 );
	voro.addPoint( ofRandom( 300, 700 ), 100 );
	voro.addPoint( ofRandom( 700, 1200 ), 100 );
	voro.addPoint( ofRandom( 1200, 1600 ), 100 );
	voro.addPoint( ofRandom( 1600, 1920 ), 100 );

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
	if( ofGetFrameNum() % 4 == 0 ) {
		plainStone.grow( *voro.getLine( 0 ) );

	}

	// slowWarpPass->setAmplitude( 0.004 );
	// slowWarpPass->setFrequency( 0.976 );

	
}

void GrowingBrushStokeAct::updateJiggle()
{
	scaleNoiseVal += 0.05f;
	rotateNoiseVal += 0.01f;
}

void GrowingBrushStokeAct::draw() {

	tintBuffer.begin();
	ofClear( 0, 0, 0, 0 );
	edgeDetectionPostProcessing.begin();
	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	ofScale( 1.6, 1.6 );
	if( doJiggle ) {
		float scale = ( ofNoise( scaleNoiseVal ) + 0.5 );
		ofScale( scale, scale );
		ofRotateZ( ( ofNoise(rotateNoiseVal) * 2 - 1 ) * 20 );
	}

	plainStone.setSelectedColor( ofColor( 255 ) );
	plainStone.setTransparency( 200 );
	plainStone.draw( -1920 / 2, -1080/2, 1920, 1080);

	ofPopMatrix();
	
	ofPopStyle();
	edgeDetectionPostProcessing.end();
	tintBuffer.end();

	//edgeDetectionPostProcessing.begin();
	ofPushStyle();
	//ofSetColor( 255, 255 );
	ofSetColor( 255, 200, 0, transparency );
	tintBuffer.draw( 0, 0 );
	ofPopStyle();
	//edgeDetectionPostProcessing.end();

	//ofPushStyle();
	//ofSetColor( 255, transparency );
	//slowWarp.draw();
	//ofPopStyle();


	//voro.draw( 0, 0 );
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

void GrowingBrushStokeAct::updateFourStones()
{
	
}

