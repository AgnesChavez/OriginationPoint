#include "EightGrowingRocks.h"
#include "ActSequencer.h"
#include "Misc.h"

EightGrowingRocks::EightGrowingRocks()
{
	rockCount = 8;

	edge.init( 1920, 1080 );
	edge.createPass< EdgePass >()->setEnabled( true );
	edge.setFlip( false );

	fbo.allocate( Misc::getDefaultFboSettings() );

	init();
}


EightGrowingRocks::~EightGrowingRocks()
{
	
}

void EightGrowingRocks::init()
{
	// clears everything so it can be re-started by calling this
	fbo.begin();
	ofClear( 0, 0, 0 );
	fbo.end();
	voro.clear();
	rocks.clear();
	transparency = 0;

	for( int i = 0; i < rockCount; i++ ) {
		BrushStone s;
		s.setBrushCollection( brushes );
		s.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
		ofPoint randomPoint( ofRandom( 100, 1820 ), ofRandom( 80, 1000 ) );
		s.init( randomPoint.x, randomPoint.y );
		voro.addPoint( randomPoint.x, randomPoint.y );
		rocks.push_back( s );
	}

	for( int i = 0; i < 10; i++ ) {
		ofPoint p1( ofRandom( 0, 1920 ), 0 );
		ofPoint p2( ofRandom( 0, 1920 ), 1080 );
		ofPoint p3( 0, ofRandom( 0, 1080 ) );
		ofPoint p4( 1920, ofRandom( 0, 1080 ) );

		voro.addPoint( p1.x, p1.y );
		voro.addPoint( p2.x, p2.y );
		voro.addPoint( p3.x, p3.y );
		voro.addPoint( p4.x, p4.y );
	}

	voro.setSmoothAmount( 20 );
	voro.compute();
	voro.render();
}

void EightGrowingRocks::update( int index)
{
	if( ofGetFrameNum() % 5 == 0 ) {
		BrushStone * stone = &rocks.at( index );
		stone->grow( *voro.getLine( index ) );
		//for( int i = 0; i < rockCount; i++ ) {
		//	BrushStone * stone = &rocks.at( i );
		//	stone->grow( *voro.getLine( i ) );
		//}
	}
}

void EightGrowingRocks::draw()
{
	fbo.begin();
	edge.begin();
	for( int i = 0; i < rockCount; i++ ) {
		rocks.at( i ).draw( 0, 0, 1920, 1080 );
	}
	edge.end();
	fbo.end();

	ofPushStyle();
	ofSetColor( 255, transparency );
	fbo.draw( 0, 0 );
	ofPopStyle();

}
