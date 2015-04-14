#include "FourGrowingStonesLayer.h"
#include "Misc.h"

FourGrowingStonesLayer::FourGrowingStonesLayer()
{
	edge.init( 1920, 1080 );
	edge.createPass< EdgePass >()->setEnabled( true );
	edge.setFlip( false );

	fbo.allocate( Misc::getDefaultFboSettings() );

	//glitch.setup( &fbo );
	//glitch.setFx( OFXPOSTGLITCH_CONVERGENCE, true );

	color = ofColor( 238, 128, 21 );

	init();
}


FourGrowingStonesLayer::~FourGrowingStonesLayer()
{
}

void FourGrowingStonesLayer::init()
{
	voroFor4Stones.clear();
	fourStonesLayer.clear();
	transparency = 0;
	fbo.begin();
	ofClear( 255, 255, 255, 0 );
	fbo.end();

	edge.begin();
	ofClear( 255, 255, 255, 0 );
	edge.end();

	ypos = 0;


	// adds four stones
	ofVec2f rand1( 650 + ofRandom( -100, 100 ), 250 );
	ofVec2f rand2( 1250 + ofRandom( -100, 100 ), 250 );
	ofVec2f rand3( 650 + ofRandom( -100, 100 ), 750 );
	ofVec2f rand4( 1250 + ofRandom( -100, 100 ), 750 );
	voroFor4Stones.addPoint( rand1.x, rand1.y );
	voroFor4Stones.addPoint( rand2.x, rand2.y );
	voroFor4Stones.addPoint( rand3.x, rand3.y );
	voroFor4Stones.addPoint( rand4.x, rand4.y );

	for( int i = 0; i < 5; i++ ) {
		voroFor4Stones.addPoint( ofRandom( 0, 1920 ), 0 );
		voroFor4Stones.addPoint( ofRandom( 0, 1920 ), 1080 );
		voroFor4Stones.addPoint( 0, ofRandom( 0, 1080 ) );
		voroFor4Stones.addPoint( 1920, ofRandom( 0, 1080 ) );
	}

	//voroFor4Stones.addPoint( 1920 / 2, 1080 / 2 );

	voroFor4Stones.setSmoothAmount( 30 );
	voroFor4Stones.compute();
	//voroFor4Stones.render();

	// creating the four brush stones
	BrushStone s;
	s.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	s.setBrushCollection( brushes );
	s.setBrushStrokeAlpha( 255 );
	s.init( rand1.x, rand1.y );

	BrushStone s2;
	s2.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	s2.setBrushCollection( brushes );
	s2.setBrushStrokeAlpha( 255 );
	s2.init( rand2.x, rand2.y );

	BrushStone s3;
	s3.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	s3.setBrushCollection( brushes );
	s3.setBrushStrokeAlpha( 255 );
	s3.init( rand3.x, rand3.y );

	BrushStone s4;
	s4.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	s4.setBrushCollection( brushes );
	s4.setBrushStrokeAlpha( 255 );
	s4.init( rand4.x, rand4.y );

	int rand = ofRandom( 0, 4 );
	switch( rand ) {
	case 0:
		fourStonesLayer.push_back( s2 );
		fourStonesLayer.push_back( s );
		fourStonesLayer.push_back( s3 );
		fourStonesLayer.push_back( s4 );
		break;
	case 1:
		fourStonesLayer.push_back( s );
		fourStonesLayer.push_back( s4 );
		fourStonesLayer.push_back( s2 );
		fourStonesLayer.push_back( s3 );
		break;
	case 2:
		fourStonesLayer.push_back( s );
		fourStonesLayer.push_back( s2 );
		fourStonesLayer.push_back( s4 );
		fourStonesLayer.push_back( s3 );
		break;
	case 3:
		fourStonesLayer.push_back( s2 );
		fourStonesLayer.push_back( s3 );
		fourStonesLayer.push_back( s4 );
		fourStonesLayer.push_back( s );
		break;
	}
	
}

void FourGrowingStonesLayer::update( int index )
{
	if( ofGetFrameNum() % 5 == 0 ) {
		BrushStone * stone = &fourStonesLayer.at( index );
		stone->grow( *voroFor4Stones.getLine( index ) );
	}
}

void FourGrowingStonesLayer::draw()
{
	fbo.begin();
	edge.begin();
	for( int i = 0; i < fourStonesLayer.size(); i++ ) {
		fourStonesLayer.at( i ).draw( 0, 0, 1920, 1080 );
	}
	edge.end();
	fbo.end();

	ofPushStyle();
	ofSetColor( color, transparency );
	//ofSetColor( 255, transparency );
	fbo.draw( 0, ypos );
	fbo.draw( 0, ypos + 1080);

	ofPopStyle();
}

void FourGrowingStonesLayer::updateYpos()
{
	ypos -= 0.5;
	if( ypos < -1080 ) {
		ypos = 0;
	}
}
