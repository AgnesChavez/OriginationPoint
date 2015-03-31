#include "ActSequencer.h"


void ActSequencer::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );
	//ofEnableSmoothing();

	TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	act1 = new StopMotionStonesAct();
	act1->stones.start();

	act2 = new GrowingBrushStokeAct();

	act2->createStone( act1->stones.centered );

	act2Transparency = 0;
	act2->transparency = act2Transparency;

	act2Ypos = 0;

	act3 = new ManyLayersAct();
	act3->transparency = 0;

	stoneCurtainXpos = 0;
}

void ActSequencer::update()
{
	unsigned long long act2Time = 270000;// 250000;
	unsigned long long act2UpdateStart = 290000;
	unsigned long long act3UpdateStart = 430000;
	unsigned long long act3MoveStoneCurtainStart = 470000;
	unsigned long long act3FourStonesMoveStart = 500000;
	
	unsigned long long currentMillis = ofGetElapsedTimeMillis();

	if( currentMillis > act2Time ) {
		act2Transparency++;
		act2->transparency = act2Transparency;
		act1->transparency = 255 - act2Transparency;
	}

	if( currentMillis > act2UpdateStart ) {
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

	if( currentMillis > act3UpdateStart ) {
		act2Ypos++;
		if( act2Ypos >= 1080 ) {
			act2Ypos = 0;
		}

		act2->transparency = -1;

		TS_START( "act3_update" );
		act3->update();
		act3->transparency = std::min(255.0, act3->transparency + 1.0);
		act3->stoneCurtainTransparency = std::min( 255.0, act3->stoneCurtainTransparency + 1.0 );
		TS_STOP( "act_3update" );
	}

	if( currentMillis > act3MoveStoneCurtainStart ) {
		act3->updateStoneCurtainPos();
	}

	if( currentMillis > act3FourStonesMoveStart ) {
		act3->updateFourStonesPos();
	}
}

void ActSequencer::draw()
{
	ofBackground( 0 );

	TS_START( "act1_draw" );
	if( act1->transparency > 0 ) {
		act1->draw();
	}
	TS_STOP( "act1_draw" );

	TS_START( "act2_draw" );
	if( act2->transparency > 0 ) {
		act2->draw();
	}
	TS_STOP( "act2_draw" );

	TS_START( "act3_draw" );
	if( act3->transparency > 0 ) {
		act3->draw();
		ofPushStyle();
		ofSetColor( 255, 255 );
		act2->tintBuffer.draw( 0, act2Ypos );
		act2->tintBuffer.draw( 0, act2Ypos - 1080 );
		ofPopStyle();
		
	}
	TS_STOP( "act3_draw" );
}

void ActSequencer::keyPressed( int key )
{
	act1->keyPressed( key );
}
