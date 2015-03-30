#include "ActSequencer.h"


void ActSequencer::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	act1 = new StopMotionStonesAct();
	act2 = new GrowingBrushStokeAct();

	act2->createStone( ofPoint( 1920/2, 1080/2 ) );

	act2Transparency = 0;
	act2->transparency = act2Transparency;

	act2Ypos = 0;

	act3 = new ManyLayersAct();
	act3->transparency = 0;
}

void ActSequencer::update()
{
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
	act1->update();
	//act2->updateFourStones();

	unsigned long long act2Time = 30000;// 250000;

	if( ofGetElapsedTimeMillis() > act2Time && ofGetFrameNum() % 1 == 0 ) {
		act2Transparency++;
		act2->transparency = act2Transparency;
		act1->transparency = 255 - act2Transparency;
		
	}

	if( ofGetElapsedTimeMillis() > act2Time + 20000 ) {
		act2->update();
		act2->transparency = 150;
	}

	if( ofGetElapsedTimeMillis() > act2Time + 140000 ) {
		act2Ypos++;
		act3->update();
		act3->transparency = 120;
	}
}

void ActSequencer::draw()
{
	ofBackground( 0, 40 );

	act1->draw();
	ofPushMatrix();
	ofTranslate( 0, act2Ypos );
	act2->draw();
	ofPopMatrix();
	act3->draw();
}

void ActSequencer::keyPressed( int key )
{
	act1->keyPressed( key );
}
