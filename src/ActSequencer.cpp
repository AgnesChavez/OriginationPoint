#include "ActSequencer.h"


void ActSequencer::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	act1 = new StopMotionStonesAct();
	act2 = new GrowingBrushStokeAct();

	act2->createStone( act1->stones.centerStone );

	act2Transparency = 0;
	act2->transparency = act2Transparency;
}

void ActSequencer::update()
{
	act1->update();

	unsigned long long act2Time = 250000;// 300000;

	if( ofGetElapsedTimeMillis() > act2Time && ofGetFrameNum() % 1 == 0 ) {
		act2Transparency++;
		act2->transparency = act2Transparency;
		act1->transparency = 255 - act2Transparency;
		
	}

	if( ofGetElapsedTimeMillis() > act2Time + 20000 ) {
		act2->update();
	}
	else {
		act2->updateJiggle();
	}
}

void ActSequencer::draw()
{
	ofBackground( 0, 40 );

	
	act1->draw();
	act2->draw();
}

void ActSequencer::keyPressed( int key )
{
	act1->keyPressed( key );
}
