#include "StopMotionStonesApp.h"

void StopMotionStonesApp::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );
	ofSetTextureWrap( GL_REPEAT );
	stones.init();

	for( int i = 0; i < 10; i++ ) {
		stones.moveRandom( 6 );
	}

	stones.setGrowing( true );

	stones.start();
}

void StopMotionStonesApp::update()
{
	stones.update();
}

void StopMotionStonesApp::draw()
{
	ofBackground( 0, 40 );
	stones.draw();
}

void StopMotionStonesApp::keyPressed( int key )
{
	if( key == 'g' ) {
		stones.setGrowing( !stones.isGrowing() );
	}
	else if( key == 'r' ) {
		stones.moveRandom( 10 );
	}
	else if( key == 'f' ) {
		ofToggleFullscreen();
	}
}
