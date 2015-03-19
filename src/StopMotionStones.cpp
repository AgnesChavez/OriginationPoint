#include "StopMotionStones.h"


StopMotionStones::StopMotionStones()
{
}


StopMotionStones::~StopMotionStones()
{
}

void StopMotionStones::init()
{
	currentStone = 0;

	int x = 15;
	int y = 10;

	stones.reserve( x * y );

	voro.clear();
	stones.clear();
	for( int i = 0; i < y; i++ ) {
		for( int j = 0; j < x; j++ ) {
			voro.addPoint( 10 + j * 130, 5 + i * 105 );
		}
	}
	voro.compute();
	for( int i = 0; i < voro.pts.size(); i++ ) {
		ofVec2f * p = &voro.pts.at( i );
		Stone s;
		s.init( p->x, p->y, *voro.getLine( i ) );
		stones.push_back( s );
	}

	stonesTex.init();
	cutter.init();
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).currentGrowRad = 25;
	}
	for( int j = 0; j < 5; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro.getLine( i ), false );
		}
	}
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).grow( voro.getLine( i ), true );
	}
	noi.render();

}

void StopMotionStones::update()
{
	currentStone += 1;
	if( currentStone >= stones.size() ) {
		currentStone = 0;
	}
}

void StopMotionStones::draw()
{
	std::vector< ofPolyline > lines;

	lines.push_back( stones.at( currentStone ).border );
	stonesTex.render( lines );
	ofFbo * buf = cutter.getCutout( noi, stonesTex.getBuffer() );
	ofPushStyle();
	ofSetColor( 243, 167, 50, 255 );
	buf->draw( 0, 0 );
	ofPopStyle();
}
