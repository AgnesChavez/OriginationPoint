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

	currentMillisTimelinePosition = 0;
	lastElapsedMillis = 0;
}

void ActSequencer::update()
{

	unsigned long long difference = ofGetElapsedTimeMillis() - lastElapsedMillis;
	lastElapsedMillis = ofGetElapsedTimeMillis();

	currentMillisTimelinePosition += difference;

	playlist.update();

	float factor = 0.05;

	unsigned long long act2Time = 220000 * factor;
	unsigned long long act2FadeInTime = act2Time + 10000 * factor;
	unsigned long long act2UpdateStart = act2FadeInTime + 10000 * factor;
	unsigned long long act2StartScaleRock = act2UpdateStart + 150000 * factor;
	unsigned long long act2UpdateFourStonesStart = act2StartScaleRock + 130000 * factor; // 427000
	unsigned long long startStoneCurtain = act2UpdateFourStonesStart + 100000 * factor; // 537000
	unsigned long long moveFourSTones = startStoneCurtain + 120000 * factor;
	unsigned long long growNewBigRock = moveFourSTones + 30000 * factor;
	unsigned long long startMoveBigRock = growNewBigRock + 20000 * factor;
	unsigned long long fadeOut4Stones = startMoveBigRock + 50000 * factor;
	unsigned long long fadeOutBigStone = fadeOut4Stones + 20000 * factor;
	unsigned long long fadeoutBackground = fadeOutBigStone + 20000 * factor;
	unsigned long long fadeoutStoneCurtain = fadeoutBackground + 20000 * factor;
	unsigned long long startOverMills = fadeoutStoneCurtain + 10000 * factor;

	std::cout << "currentMillisTimelinePosition: " << currentMillisTimelinePosition << std::endl;

	if( currentMillisTimelinePosition > act2Time ) {
		act1->transparency--;
	}

	if( currentMillisTimelinePosition > act2FadeInTime ) {
		act2->transparency += 0.3;
		act2->transparency = std::min( 170.0f, act2->transparency );
	}

	if( currentMillisTimelinePosition > act2UpdateStart ) {
		
		act2->update();

		if( currentMillisTimelinePosition > act2StartScaleRock  ) {
			act2->doScale = true;
		}

		if( currentMillisTimelinePosition > act2UpdateFourStonesStart ) {
			int padding = 10000;
			for( int i = 0; i < 4; i++ ) {
				if( currentMillisTimelinePosition > act2UpdateFourStonesStart + i * padding ) {
					act2->fourRocks.update( i );
				}
			}

			act2->fourRocks.transparency += 0.3;
			act2->fourRocks.transparency = std::min( 140.0f, act2->fourRocks.transparency );
		}
	}
	else {
		act1->update();
	}

	if( currentMillisTimelinePosition > startStoneCurtain && currentMillisTimelinePosition < fadeoutBackground ) {

		act3->updateStoneCurtainPos();
		act3->stoneCurtainTransparency += 0.5f;
		act3->stoneCurtainTransparency = std::min( 190.0f, act3->stoneCurtainTransparency );
	}

	if( currentMillisTimelinePosition > moveFourSTones ) {
		act2->fourRocks.updateYpos();
	}

	if( currentMillisTimelinePosition > growNewBigRock && currentMillisTimelinePosition < fadeoutStoneCurtain ) {
		act2->updateSecondStone();
		act2->secondPlainStoneTransparency += 0.5f;
		act2->secondPlainStoneTransparency = std::min( act2->secondPlainStoneTransparency, 130.0f );
	}

	if( currentMillisTimelinePosition > startMoveBigRock ) {
		act2->updateRockYpos();
	}

	if( currentMillisTimelinePosition > fadeOut4Stones ) {
		act2->fourRocks.transparency -= 0.5f;
		act2->fourRocks.transparency = std::max( act2->fourRocks.transparency, 0.0f );
		std::cout << "fade out four rocks to " << act2->fourRocks.transparency << std::endl;
	}

	if( currentMillisTimelinePosition > fadeOutBigStone ) {
		act2->transparency -= 1.0f;
		std::cout << "fade out big stone to " << act2->transparency << std::endl;
		act2->transparency = std::max( act2->transparency, 1.5f );
	}

	if( currentMillisTimelinePosition > fadeoutBackground ) {
		act2->secondPlainStoneTransparency -= 0.5f;
		act2->secondPlainStoneTransparency = std::max( act2->secondPlainStoneTransparency, 0.0f );
		std::cout << "fade out second big stone to " << act2->secondPlainStoneTransparency << std::endl;
	}

	if( currentMillisTimelinePosition > fadeoutStoneCurtain ) {
		act3->stoneCurtainTransparency -= 1.0f;
		act3->stoneCurtainTransparency = std::max( act3->stoneCurtainTransparency, 0.0f );
		std::cout << "fade out stone curtain to " << act3->stoneCurtainTransparency << std::endl;
	}

	if( currentMillisTimelinePosition > startOverMills && currentMillisTimelinePosition < startOverMills + 500 ) {
		currentMillisTimelinePosition = 0;
		std::cout << "Finished one cycle. yay." << std::endl;
		act1->setup();
		act1->stones.start();
		act2->setup();
		act3->setup();
	}
}

void ActSequencer::draw()
{
	ofBackground( 0 );

	ofPushMatrix();
	ofMatrix4x4 mat = warper.getMatrix();
	ofMultMatrix( mat );

	if( act1->transparency > 0 ) {
		act1->draw();
	}

	if( act2->transparency > 0 ) {
		act2->draw();
		if( act2->fourRocks.transparency > 1 ) {
			act2->fourRocks.draw();
		}
	}

	if( act3->stoneCurtainTransparency > 0 ) {
		act3->draw();
	}

	act2->drawSecondStone();

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
