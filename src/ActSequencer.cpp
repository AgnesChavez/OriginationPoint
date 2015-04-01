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

	testVal = 0.0;
	playlist.addKeyFrame( Playlist::Action::tween( 200.0f, &testVal, 100.0 ) );
	playlist.addKeyFrame( Playlist::Action::tween( 200.0f, &testVal, 300.0 ) );

	int x = 0;
	int y = 0;
	int w = 1920;
	int h = 1080;
	warper.setSourceRect( ofRectangle( 0, 0, ofGetWidth(), ofGetHeight() ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
	warper.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
	warper.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
	warper.setup();
	warper.load(); // reload last saved changes.
}

void ActSequencer::update()
{

	playlist.update();
	std::cout << testVal << std::endl;

	unsigned long long act2Time = 270000;
	unsigned long long act2FadeInTime = 280000;
	unsigned long long act2UpdateStart = 290000;
	unsigned long long act2UpdateSixStonesStart = 430000;
	unsigned long long act3UpdateStart = 570000;
	unsigned long long act3MoveStoneCurtainStart = 610000;
	unsigned long long act3FourStonesMoveStart = 640000;
	
	unsigned long long currentMillis = ofGetElapsedTimeMillis();

	if( currentMillis > act2Time ) {
		act2Transparency++;
		act1->transparency = 255 - act2Transparency;
	}

	if( currentMillis > act2FadeInTime ) {
		act2->transparency = act2Transparency;
	}

	if( currentMillis > act2UpdateStart ) {
		TS_START( "act2_update" );
		act2->update();
		TS_STOP( "act2_update" );
		act2->transparency = 255;
		if( currentMillis > act2UpdateSixStonesStart ) {
			act2->sixRocks.update();
			act2->sixRocks.transparency++;
		}
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

	ofPushMatrix();
	ofMatrix4x4 mat = warper.getMatrix();
	ofMultMatrix( mat );

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

	ofPopMatrix();
	ofPushStyle();

	ofSetColor( ofColor::magenta );
	warper.drawQuadOutline();

	ofSetColor( ofColor::yellow );
	warper.drawCorners();

	ofSetColor( ofColor::magenta );
	warper.drawHighlightedCorner();

	ofSetColor( ofColor::red );
	warper.drawSelectedCorner();
	ofPopStyle();
}

void ActSequencer::keyPressed( int key )
{
	act1->keyPressed( key );

	switch( key ) {
	case ' ':
		warper.toggleShow();
		break;
	}
}

std::vector< ofPoint > ActSequencer::getLineSplitPoints( ofPolyline linesToSplit, float stepLength )
{
	std::vector< ofPoint > returnPoints;
	returnPoints.push_back( linesToSplit.getVertices().at( 0 ) );

	ofPolyline line = linesToSplit;
	ofPoint last = line.getVertices().at( 0 );
	for( int j = 0; j < line.getVertices().size(); j++ ) {
		ofPoint p = line.getVertices().at( j );
		float dist = p.distance( last );
		if( dist > stepLength ) {
			ofPoint pos = last;
			ofPoint step = ( p - last ).limit( stepLength );
			while( dist > stepLength ) {
				pos += step;
				returnPoints.push_back( pos );
				dist -= stepLength;
			}
		}

		last = p;
	}
	returnPoints.push_back( line.getVertices().at( line.getVertices().size() - 1 ) );

	return returnPoints;
}

void ActSequencer::drawSplitLines( std::vector< ofPoint > points )
{
	ofPoint prev;
	for( int i = 0; i < points.size(); i++ ) {
		ofPoint p = points.at( i );
		if( i % 2 == 0 ) {
			prev = p;
		}
		else {
			if( prev.x >= 0 && prev.x <= 1920 && prev.y >= 0 && prev.y <= 1080 ) {
				if( p.x >= 0 && p.x <= 1920 && p.y >= 0 && p.y <= 1080 ) {
					ofLine( prev, p );
				}
			}
		}
	}
}
