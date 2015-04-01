#include "SixGrowingRocksLayer.h"
#include "ActSequencer.h"

SixGrowingRocksLayer::SixGrowingRocksLayer()
{
	rockCount = 6;

	edge.init( 1920, 1080 );
	edge.createPass< EdgePass >()->setEnabled( true );
	edge.setFlip( false );

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	fbo.allocate( settings );

	for( int i = 0; i < rockCount; i++ ) {
		std::vector< ofPoint > emptyPoints;
		dottedPoints.push_back( emptyPoints );
	}

	init();
}


SixGrowingRocksLayer::~SixGrowingRocksLayer()
{
	
}

void SixGrowingRocksLayer::init()
{
	// clears everything so it can be re-started by calling this
	fbo.begin();
	ofClear( 0, 0, 0 );
	fbo.end();
	voro.clear();
	rocks.clear();
	transparency = 0;
	for( int i = 0; i < rockCount; i++ ) {
		dottedPoints.at( i ).clear();
	}


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

void SixGrowingRocksLayer::update()
{
	for( int i = 0; i < rockCount; i++ ) {
		BrushStone * stone = &rocks.at( i );
		stone->grow( *voro.getLine( i ) );
	}

	for( int i = 0; i < rockCount; i++ ) {
		ofPolyline lineToDraw = *voro.getLine( i ); // rocks.at( i ).border
		std::vector< ofPoint > pointsToDraw = ActSequencer::getLineSplitPoints( lineToDraw, 4 );
		dottedPoints.at( i ) = pointsToDraw;
	}
}

void SixGrowingRocksLayer::draw()
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

	for( int i = 0; i < rockCount; i++ ) {
		//voro.getLine( i )->draw();
		ActSequencer::drawSplitLines( dottedPoints.at( i ) );
	}
	
	ofPopStyle();
}
