#include "RandomWalkLayer.h"
#include "Misc.h"

RandomWalkLayer::RandomWalkLayer()
{
	randomWalkerCount = 35;
	stepLength = 5;
	walkSpeed = 5;
	transparency = 140;

	lineWeidthNoiseVal = 0.0;
	colorNoiseVal = 0.0;
	transparencyNoiseVal = 0.0;

	buffer.allocate( Misc::getDefaultFboSettings() );

	for( int i = 0; i < randomWalkerCount; i++ )
	{
		currentPositions.push_back( ofPoint(ofRandom( 0, 1920 ), ofRandom( 0, 1080 ) ) );
		lastPositions.push_back( currentPositions.at( i ) );
	}

	buffer.begin();
	ofClear( 0.0, 0.0, 0.0, 0.0 );
	buffer.end();
}


RandomWalkLayer::~RandomWalkLayer()
{
}

void RandomWalkLayer::update()
{
	for( int j = 0; j < walkSpeed; j++ ) {

		lineWeidthNoiseVal += 0.01;
		colorNoiseVal += 0.01;
		transparencyNoiseVal += 0.01;

		for( auto i = 0; i < randomWalkerCount; i++ )
		{
			ofPoint pAdd( ofRandom( -stepLength, stepLength ), ofRandom( -stepLength, stepLength ) );
			currentPositions.at( i ) += pAdd;
		}

		buffer.begin();
		ofPushStyle();
		for( auto i = 0; i < randomWalkerCount; i++ )
		{
			ofPoint curr( currentPositions.at( i ) );
			ofPoint last( lastPositions.at( i ) );

			ofNoFill();
			float col = ofNoise( colorNoiseVal ) * 255;
			float trans = ofNoise( transparencyNoiseVal ) * 255;
			ofSetColor( col, trans );
			float lineW = ofNoise( lineWeidthNoiseVal ) * 10;
			glLineWidth( lineW );
			ofLine( curr.x, curr.y, last.x, last.y );
		}
		ofPopStyle();
		buffer.end();

		for( auto i = 0; i < randomWalkerCount; i++ )
		{
			lastPositions.at( i ) = currentPositions.at( i );
		}

	}
}

void RandomWalkLayer::draw()
{
	ofPushStyle();
	ofSetColor( 255, transparency );
	buffer.draw( 0, 0, 1920, 1080 );
	ofPopStyle();
}
