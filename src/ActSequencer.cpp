#include "ActSequencer.h"


void ActSequencer::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( false );
	ofSetFrameRate( 30 );

	TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	act1 = new StopMotionStonesAct();
	act2 = new GrowingBrushStokeAct();

	act2->createStone( act1->stones.centered );

	act2Transparency = 0;
	act2->transparency = act2Transparency;

	act2Ypos = 0;

	act3 = new ManyLayersAct();
	act3->transparency = 0;
}

void ActSequencer::update()
{
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );

	unsigned long long act2Time = 40000;// 250000;
	

	if( ofGetElapsedTimeMillis() > act2Time) {
		act2Transparency++;
		act2->transparency = act2Transparency;
		act1->transparency = 255 - act2Transparency;
	}

	if( ofGetElapsedTimeMillis() > act2Time + 20000 ) {
		TS_START( "act2_update" );
		act2->update();
		TS_STOP( "act2_update" );
		act2->transparency = 150;
	}
	else {
		TS_START( "act1_update" );
		act1->update();
		TS_STOP( "act1_update" );
	}

	if( ofGetElapsedTimeMillis() > act2Time + 140000 ) {
		act2Ypos++;
		TS_START( "act3_update" );
		act3->update();
		TS_STOP( "act_3update" );
		act3->transparency = 120;
	}
}

void ActSequencer::draw()
{
	ofBackground( 0, 40 );

	TS_START( "act1_draw" );
	act1->draw();
	TS_STOP( "act1_draw" );

	TS_START( "act2_draw" );
	ofPushMatrix();
	ofTranslate( 0, act2Ypos );
	act2->draw();
	ofPopMatrix();
	TS_STOP( "act2_draw" );

	TS_START( "act3_draw" );
	act3->draw();
	TS_STOP( "act3_draw" );
}

void ActSequencer::keyPressed( int key )
{
	act1->keyPressed( key );
}
