#include "StopMotionStones.h"


StopMotionStones::StopMotionStones() :
	x( 30 ),
	y( 20 ),
	xSpacing( 1920 / x ),
	ySpacing( 1080 / y ),
	transparency( 255 )
{
	voro = new VoronoiLayer();
	voro->setSmoothAmount( 10 );

	waterEffect = new ofxPostProcessing();

	stonesTex.init();
	cutter.init();
	noi.render();
}

StopMotionStones::~StopMotionStones()
{
	delete voro;
	delete waterEffect;
}

void StopMotionStones::init()
{
	lastMove1 = lastMove2 = lastMove3 = 0;
	currentStone = 0;
	secondCurrentStone = 0;
	thirdCurrentStone = 0;
	doGrow = false;
	transparency = 255;
	isStarted = false;
	flickeringStonesRelativeTransparency = 0.0;

	//cutter.init();

	voro->clear();
	stones.clear();
	transparencies.clear();
	voronoiCentroids.clear();
	stones.reserve( x * y );
	toDrawStone.clear();

	/* Creates grid aligned voronoi diagram */
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
		s.maxGrowRad = 150;
		stones.push_back( s );
	}

	for( int j = 0; j < 3; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).currentGrowRad = 10;
			stones.at( i ).grow( voro->getLine( i ), true );
		}
	}

	centered = getMostCenteredVoronoiCellCenter();

	for( int i = 0; i < x*y; i++ ) {
		transparencies.push_back( 255 );
	}

	delete waterEffect;
	waterEffect = new ofxPostProcessing();
	waterEffect->init( 1920, 1080 );
	waterEffect->setFlip( false );
	waterEffectPointer = waterEffect->createPass<NoiseWarpPass>();
	waterEffectPointer->setEnabled( false );
	waterEffectPointer->setAmplitude( 0.004 );
	waterEffectPointer->setFrequency( 0.976 );
}

void StopMotionStones::start()
{
	isStarted = true;
	//startedMillis = ofGetElapsedTimeMillis();
}

void StopMotionStones::update( unsigned long long millis )
{
	long millisStopMotionPart1 = 30000;
	long millisStopMotionPart1AndAHalf = 40000;
	long millisStopMotionPart2 = 70000;
	long millisBrownianMotionPart1 = 110000;
	long millisBrownianMotionPart2 = 150000;
	long millisStartFadeAllOut = 158000;

	if( isStarted ) {
		if( isWithinMillis( millis, 0, millisStopMotionPart1 ) ) {
			toDrawStone.clear();

			currentStone += 1;
		}
		if( isWithinMillis( millis, millisStopMotionPart1, millisStopMotionPart1AndAHalf ) ) {
			toDrawStone.clear();
			currentStone += 1;
			secondCurrentStone += 2;
		}
		if( isWithinMillis( millis, millisStopMotionPart1AndAHalf, millisStopMotionPart2 ) ) {
			toDrawStone.clear();
			secondCurrentStone += 1;
			currentStone += 2;
			thirdCurrentStone += 4;

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
			currentStone = getIndexFrom2D( ofPoint( _xIndex, _yIndx ) );
		}
		if( isWithinMillis( millis, millisStopMotionPart2, millisBrownianMotionPart1 ) ) {
			toDrawStone.clear();
			currentStone = doBrownianMotion( currentStone, 0 );
			secondCurrentStone = doBrownianMotion( secondCurrentStone, 1 );
			thirdCurrentStone = doBrownianMotion( thirdCurrentStone, 2 );
		}
		if( isWithinMillis( millis, millisBrownianMotionPart1, millisBrownianMotionPart2 ) ) {
			currentStone = doBrownianMotion( currentStone, 0 );
			secondCurrentStone = doBrownianMotion( secondCurrentStone, 1 );
			thirdCurrentStone = doBrownianMotion( thirdCurrentStone, 2 );
			flickeringStonesRelativeTransparency--;
		}
		if( isWithinMillis( millis, millisBrownianMotionPart2, millisStartFadeAllOut ) ) {
			for( int i = 0; i < 15; i++ ) {
				int rand = static_cast< int >( ofRandom( x * y ) );
				toDrawStone.insert( rand );
			}
		}

		if( isPastMillis( millis, millisStartFadeAllOut ) ) {
				
			for( int i = 0; i < 35; i++ ) {
				int rand = ( int ) ( ofRandom( x * y ) );
				transparencies.at( rand ) -= 2;
			}
		}
	}

	if( currentStone >= stones.size() ) {
		currentStone = 0;
	}
	if( secondCurrentStone >= stones.size() ) {
		secondCurrentStone = 0;
	}
	if( thirdCurrentStone >= stones.size() ) {
		thirdCurrentStone = 0;
	}

	toDrawStone.insert( currentStone );
	toDrawStone.insert( secondCurrentStone );
	toDrawStone.insert( thirdCurrentStone );
	removeOuterEdges();

	selectedLines.clear();

	// select lines to be rendered
	for( std::set<int>::iterator it = toDrawStone.begin(); it != toDrawStone.end(); ++it ) {
		int inde = *it;
		selectedLines.push_back( stones.at( inde ).border );
	}

	stonesTex.render( selectedLines, transparencies, centered );
}

void StopMotionStones::draw()
{
	ofFbo * buf = cutter.getCutout( noi, stonesTex.getBuffer() );
	waterEffect->begin();
	ofPushStyle();
	ofSetColor( 255, transparency );
	ofBackground( 0 );
	buf->draw( 0, 0 );
	//ofSetColor( 255, 0, 0, transparency );
	drawCustomVoronoi();
	ofPopStyle();
	waterEffect->end();
}

void StopMotionStones::drawCustomVoronoi()
{
	ofPushStyle();
	ofSetColor( 255, 0, 0, transparency );
	std::vector< ofVec2f > pts = voro->getPoints();
	for( std::set<int>::iterator it = toDrawStone.begin(); it != toDrawStone.end(); ++it ) {
		int inde = *it;
		int _x = inde % x;
		int _y = ( inde - _x ) / x;
		// excludes cell centers at the edges
		ofVec2f pt = pts[ inde ];
		if( pt.x > 95 && pt.x < 1860 && pt.y > 68 && pt.y < 1037 ) {
				ofSetColor( 0, transparencies.at( inde ) );
				ofCircle( pts[ inde ].x, pts[ inde ].y, 4 );
		}
	}

	for( int i = 0; i < voro->pts.size(); i++ ) {
		ofPoint cent = voro->pts.at( i);
		ofSetColor( ofRandom( 40, 100 ), 200 + flickeringStonesRelativeTransparency );
		ofCircle( cent.x, cent.y, 1 );
	}

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

void StopMotionStones::removeOuterEdges()
{
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

bool StopMotionStones::isWithinMillis( unsigned long long currentSystemMillis, unsigned long long start, unsigned long long end )
{
	unsigned long long currentMillis = currentSystemMillis;
	return currentMillis - startedMillis > start && currentMillis - startedMillis < end;
}

int StopMotionStones::doBrownianMotion( int currStone, int which )
{
	int lastMove = 0;
	if( which == 0 ) {
		lastMove = lastMove1;
	}
	else if( which == 1 ) {
		lastMove = lastMove2;
	}
	else if( which == 2 ) {
		lastMove = lastMove3;
	}

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

	if( which == 0 ) {
		lastMove1 = lastMove;
	}
	else if( which == 1 ) {
		lastMove2 = lastMove;
	}
	else if( which == 2 ) {
		lastMove3 = lastMove;
	}

	int ind = getIndexFrom2D( index2d );

	return ind;
}

bool StopMotionStones::isPastMillis( unsigned long long currentSystemMillis, unsigned long long mill )
{
	unsigned long long currentMillis = currentSystemMillis;
	return currentMillis - startedMillis > mill;
}

ofPoint StopMotionStones::getMostCenteredVoronoiCellCenter()
{
	float distance = 1000;
	ofPoint selectedVoronoiCenter;
	for( int i = 0; i < stones.size(); i++ ) {
		ofPoint cent = stones.at( i ).border.getCentroid2D();
		float d = ofDist( 1920 / 2, 1080 / 2, cent.x, cent.y );
		if( d < distance ) {
			distance = d;
			selectedVoronoiCenter = cent;
		}
	}

	return selectedVoronoiCenter;
}

int StopMotionStones::getMostCenteredVoronoiStoneIndex()
{
	float distance = 1000;
	int index = 0;
	for( int i = 0; i < stones.size(); i++ ) {
		ofPoint cent = stones.at( i ).border.getCentroid2D();
		float d = ofDist( 1920 / 2, 1080 / 2, cent.x, cent.y );
		if( d < distance ) {
			distance = d;
			index = i;
		}
	}

	return index;
}

void StopMotionStones::detectedStoneCount( int count )
{
	switch( count ) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}
}


