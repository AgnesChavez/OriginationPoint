#include "StopMotionStonesAct.h"

StopMotionStonesAct::StopMotionStonesAct() :
transparency( 255 )
{
	setup();
}

StopMotionStonesAct::~StopMotionStonesAct()
{

}

void StopMotionStonesAct::setup()
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

	transparency = 255;

	stones.start();
}

void StopMotionStonesAct::update()
{
	stones.transparency = transparency;
	stones.update();
}

void StopMotionStonesAct::draw()
{
	stones.draw();
}

void StopMotionStonesAct::keyPressed( int key )
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

