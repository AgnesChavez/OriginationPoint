#include "StopMotionStones.h"


StopMotionStones::StopMotionStones()
{
	
}

StopMotionStones::~StopMotionStones()
{
}

void StopMotionStones::init()
{
	lastMove = 0;
	currentStone = 0;
	doGrow = false;

	/* Creates grid aligned voronoi diagram */

	voro = new VoronoiLayer();
	voro->setSmoothAmount( 10 );

	x = 30;
	y = 20;

	int xSpacing = 1920 / x;
	int ySpacing = 1080 / y;

	stones.reserve( x * y );

	voro->clear();
	stones.clear();
	for( int i = 0; i < y; i++ ) {
		for( int j = 0; j < x; j++ ) {
			voro->addPoint( ( 50 + j * xSpacing ) + ofRandom( -5, 5 ), ( 40 + i * ySpacing ) + ofRandom( -5, 5 ) );
		}
	}
	voro->compute();
	for( int i = 0; i < voro->pts.size(); i++ ) {
		ofVec2f * p = &voro->pts.at( i );
		Stone s;
		s.init( p->x, p->y, *voro->getLine( i ) );
		stones.push_back( s );
	}

	/* Creating a second voronoi diagram in order to move the stones which
	are aligned into a grid into the positions of this random voronoi diagram */

	secondVoro = new VoronoiLayer();

	for( int i = 0; i < x * y; i++ ) {
		secondVoro->addRandomPoint();
	}

	secondVoro->compute();

	/* Done second voronoi init */

	stonesTex.init();
	cutter.init();

	for( int j = 0; j < 3; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).currentGrowRad = 10;
			stones.at( i ).grow( voro->getLine( i ), true );
		}
	}
	noi.render();
}

void StopMotionStones::update()
{
	/* Doing random jumping */
	if( ofGetFrameNum() < 200 ) {
		currentStone += 1;
		if( currentStone >= stones.size() ) {
			currentStone = 0;
		}
	}
	else if( ofGetFrameNum() > 200 && ofGetFrameNum() < 400 ) {
		currentStone += 2;
		ofPoint index2d = get2DFromIndex( currentStone );
		
		int _xIndex = index2d.x;
		int _yIndx = index2d.y;
		float rand = ofRandom( -1, 1 );
		if( rand > 0.95f ) {
			_yIndx++;
		}
		else if( rand < -0.95f ) {
			_yIndx--;
		}
		currentStone = getIndexFrom2D( ofPoint(_xIndex, _yIndx ) );
		if( currentStone >= stones.size() ) {
			currentStone = 0;
		}
	}
	else if( ofGetFrameNum() > 400  ) {
		float rand = ofRandom( 4 );
		ofPoint index2d = get2DFromIndex( currentStone );
		if( rand > 0 && rand < 1 && lastMove != 2 ) {
			index2d.x += 1;
			lastMove = 1;
		}
		else if( rand > 1 && rand < 2 && lastMove != 1 ) {
			index2d.x -= 1;
			lastMove = 2;
		}
		else if( rand > 2 && rand < 3 && lastMove != 4 ) {
			index2d.y -= 1;
			lastMove = 3;
		}
		else if( rand > 3 && rand < 4 && lastMove != 3 ) {
			index2d.y += 1;
			lastMove = 4;
		}

		int ind = getIndexFrom2D( index2d );
		
		currentStone = ind;
		if( currentStone >= stones.size() ) {
			currentStone = 0;
		}
	}

	if( ofGetFrameNum() > 600 ) {
		toDrawStone.insert( currentStone );
	}

	if( doGrow && ofGetFrameNum() % 3 == 0 ) {
		for( std::set<int>::iterator it = toDrawStone.begin(); it != toDrawStone.end(); ++it ) {
			Stone * s = &stones.at( *it );
			s->grow( voro->getLine( *it ), true );
			int ind = *it;
			ofPoint p = get2DFromIndex( ind );
			if( p.x == 0 ) {
				toDrawStone.erase( it );
			}
			else if( p.x == x - 1 ) {
				toDrawStone.erase( it );
			}
			else if( p.y == 0 ) {
				toDrawStone.erase( it );
			}
			else if( p.y == y - 1 ) {
				toDrawStone.erase( it );
			}
		}
	}

	if( ofGetFrameNum() > 1500 ) {
		for( int i = 0; i < 2; i++ ) {
			int rand = static_cast<int>(ofRandom( x * y ) );
			toDrawStone.insert( rand );
		}
	}

	std::vector< ofPolyline > lines;

	lines.push_back( stones.at( currentStone ).border );

	// select lines to be rendered
	for( std::set<int>::iterator it = toDrawStone.begin(); it != toDrawStone.end(); ++it ) {
		int inde = *it;
		lines.push_back( stones.at( inde ).border );
	}

	stonesTex.render( lines );
}

void StopMotionStones::draw()
{
	
	ofFbo * buf = cutter.getCutout( noi, stonesTex.getBuffer() );
	ofPushStyle();
	ofSetColor( 255, 255 );
	buf->draw( 0, 0 );
	ofPopStyle();

	ofPushStyle();
	voro->setLineThickness( 0.1 );
	ofSetColor( 255, 0, 0 );
	voro->render();
	voro->draw( 0, 0 );
	ofPopStyle();
}

void StopMotionStones::moveRandom( float str )
{
	/* Trying to move stones away from their grid position */

	for( std::vector<ofVec2f>::iterator it = voro->pts.begin(); it != voro->pts.end(); ++it ) {
		it->x = it->x + ofRandom( -str, str );
		it->y = it->y + ofRandom( -str, str );
	}

	voro->compute();

	/*
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
	*/
}

void StopMotionStones::setGrowing( bool gr )
{
	this->doGrow = gr;
}

ofPoint StopMotionStones::get2DFromIndex( int index )
{
	int _x = index % x;
	int _y = ( index - _x ) / x;
	return ofPoint( _x, _y );
}

int StopMotionStones::getIndexFrom2D( ofPoint d )
{
	int _x = static_cast<int>( d.x );
	int _y = static_cast<int>( d.y );

	int index = _y * x + _x;
	return index;
}

bool StopMotionStones::isGrowing()
{
	return doGrow;
}
