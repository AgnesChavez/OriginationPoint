#include "StopMotionStonesAct.h"

StopMotionStonesAct::StopMotionStonesAct()
{
	setup();
}

StopMotionStonesAct::~StopMotionStonesAct()
{

}

void StopMotionStonesAct::setup()
{
	stones.init();

	for( int i = 0; i < 10; i++ ) {
		stones.moveRandom( 6 );
	}

	stones.setGrowing( true );

	transparency = 255;

	stones.start();

	vectorField.setup( 1920, 1080, 5 );
	vectorField.randomize();
}

void StopMotionStonesAct::update( unsigned long long millis )
{
	stones.transparency = transparency;
	stones.update( millis );
}

void StopMotionStonesAct::draw()
{
	stones.draw();

	ofPushStyle();
	vectorField.animate( 0.008 );
	vectorField.draw( stones.transparency - 170 );
	ofPopStyle();
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

