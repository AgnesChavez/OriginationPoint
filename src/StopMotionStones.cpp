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
	doGrow = false;

	voro = new VoronoiLayer();
	voro->setSmoothAmount( 10 );

	int x = 15;
	int y = 10;

	stones.reserve( x * y );

	voro->clear();
	stones.clear();
	for( int i = 0; i < y; i++ ) {
		for( int j = 0; j < x; j++ ) {
			voro->addPoint( 10 + j * 130, 5 + i * 105 );
		}
	}
	voro->compute();
	for( int i = 0; i < voro->pts.size(); i++ ) {
		ofVec2f * p = &voro->pts.at( i );
		Stone s;
		s.init( p->x, p->y, *voro->getLine( i ) );
		stones.push_back( s );
	}

	stonesTex.init();
	cutter.init();
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).currentGrowRad = 25;
	}
	for( int j = 0; j < 5; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro->getLine( i ), false );
		}
	}
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).grow( voro->getLine( i ), true );
	}
	noi.render();

}

void StopMotionStones::update()
{
	currentStone += 1;
	if( currentStone >= stones.size() ) {
		currentStone = 0;
	}

	if( doGrow && ofGetFrameNum() % 10 == 0 ) {
		for( int i = 0; i < stones.size(); i++ ) {
			Stone * s = &stones.at( i );
			s->grow( voro->getLine( i ), true );
		}
	}
}

void StopMotionStones::draw()
{
	std::vector< ofPolyline > lines;

	int nrToDraw = ( int ) ( ofMap(ofGetFrameNum(), 0, 1000, 0, stones.size() / 2 ) );
	std::cout << "nrtodraw:" << nrToDraw << std::endl;
	int from = currentStone - nrToDraw;
	int to = currentStone + nrToDraw;

	std::cout << "from: " << from << " to: " << to << std::endl;

	lines.push_back( stones.at( currentStone ).border );

	for( int i = from; i < to; i++ ) {
		int in = i % 150;
		if( i < 0 ) in = std::abs( i );
		lines.push_back( stones.at( in ).border );
	}

	if( doGrow ) {
		for( int i = 0; i < stones.size(); i++ ) {
			lines.push_back( stones.at( i ).border );
		}
	}

	stonesTex.render( lines );
	ofFbo * buf = cutter.getCutout( noi, stonesTex.getBuffer() );
	ofPushStyle();
	ofSetColor( 243, 167, 50, 255 );
	buf->draw( 0, 0 );
	ofPopStyle();
}

void StopMotionStones::moveRandom( float str )
{

	stones.clear();
	std::vector< ofPoint > cents;

	for( int i = 0; i < voro->pts.size(); i++ ) {
		ofPoint transp( ofRandom( -str, str ), ofRandom( -str, str ) );
		voro->pts.at( i ).x += transp.x;
		voro->pts.at( i ).y += transp.y;
		transp += voro->pts.at( i );
		cents.push_back( transp );
	}

	voro->clear();

	for( int i = 0; i < cents.size(); i++ ) {
		voro->addPoint( cents.at( i ).x, cents.at( i ).y );
	}

	voro->compute();
	for( int i = 0; i < voro->pts.size(); i++ ) {
		ofVec2f * p = &voro->pts.at( i );
		Stone s;
		s.init( p->x, p->y, *voro->getLine( i ) );
		stones.push_back( s );
	}

	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).currentGrowRad = 25;
	}

	int timesss = ( int ) ( ofMap( ofGetFrameNum(), 0, 1000, 5, 100 ) );
	timesss = std::min( 100, timesss );
	std::cout << timesss << std::endl;
	for( int j = 0; j < timesss; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro->getLine( i ), false );
		}
	}
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).grow( voro->getLine( i ), true );
	}

}

void StopMotionStones::setGrowing( bool gr )
{
	this->doGrow = gr;
}
