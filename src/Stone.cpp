#include "Stone.h"
#include "omp.h"


Stone::Stone( )
{
	setFuzzy( 10.0f );
	setRadius( 40.0f );
	setSize( 60 );
	transparency = 255;
	borderTransparency = 255;
	saturation = 255;

	borderSize = 30;

	currentGrowRad = 10;
	maxGrowRad = 250;
}

Stone::~Stone()
{
}

void Stone::init( float _x, float _y, ofPolyline line  )
{
	bufferWidth = 1920 / 20.0;
	bufferHeight = 1080 / 20.0;


	tDrawStone = true;
	tDrawBorder = false;
	
	centroid = ofPoint( ofMap(_x, 0, 1920, 0, bufferWidth ), ofMap( _y, 0, 1080, 0, bufferHeight ) );
}

void Stone::clear()
{
	TS_START( "stone_indi_clear" );

	locationsPointsDrawn.clear();
	contourPoints.clear();

	currentGrowRad = 10.0f;
		
	TS_STOP( "stone_indi_clear" );
}

void Stone::grow( ofPolyline line, bool renderbrush, bool calcDirectly )
{
	if( currentGrowRad < maxGrowRad ) {
		currentGrowRad += 0.5f;

		int nrToCheck = ( int ) ( ofMap( currentGrowRad, 10, maxGrowRad, 5, 15 ) );
		
		std::vector< ofVec2f > pointsToDraw( nrToCheck );
		ofVec2f p = line.getCentroid2D();
		for( int i = 0; i < nrToCheck; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			//int randomId = ofRandom( 0, points.size() );
			
			ofVec2f pToSave = p + ofVec2f( _x, _y );
			pointsToDraw.at( i ) = pToSave;
		}

		ofPolyline lineToCheck = line;// .getResampledBySpacing( 5 );
		std::vector< ofPoint > newPointsToDraw( nrToCheck);

		// checking parallelized for overlapping
		for( int i = 0; i < pointsToDraw.size(); i++ ) {
			ofVec2f p = pointsToDraw.at( i );
			ofRectangle bb = lineToCheck.getBoundingBox();
			// first check for bounding box inside as its quicker to compute
			if( bb.inside( p ) ) {
				if( lineToCheck.inside( p ) ) {
					newPointsToDraw.at( i ) = ofPoint( p.x, p.y );
				}
			}
		}

		// removing vectors at around location 0, 0
		vector<ofPoint>::iterator it = newPointsToDraw.begin();
		for( ; it != newPointsToDraw.end(); ) {
			ofPoint *_p = &( *it );
			if( _p->x < 4 || _p->y < 4 ) {
				it = newPointsToDraw.erase( it );
			}
			else {
				++it;
				locationsPointsDrawn.push_back( *_p );
			}
		}

		if( renderbrush ) {
			renderBrushStone( newPointsToDraw );
		}

		if( calcDirectly ) {
			calcBorder( locationsPointsDrawn );
			renderBorder();
		}

		if( tDrawBorder ) {
			renderBorder();
		}
	}
}

void Stone::calcBorder( std::vector< ofPoint > points )
{
	if( points.size() > 3 ) {
		contourPoints = convexHull.getConvexHull( points );
	}
}

void Stone::renderBorder()
{
	if( contourPoints.size() > 0 ) {
		ofFill();

		std::vector< Point1 > convexPoints( contourPoints.size() );
		for( int i = 0; i < contourPoints.size(); i += 1 ) {
			ofPoint _poi = contourPoints.at( i );
			Point1 p;
			p.x = _poi.x;
			p.y = _poi.y;
			convexPoints.at( i ) = p;
		}
		std::vector< Point1 > ps = VoronoiLayer::convex_hull( convexPoints );

		border.clear();
		border.setClosed( true );

		std::vector< ofPoint > finalPoints( ps.size() );
		for( int i = 0; i < ps.size(); i++ ) {
			Point1 from = ps.at( i );
			finalPoints.at( i ) = ofPoint( from.x, from.y );
		}

		border.addVertices( finalPoints );

		border.setClosed( true );
		border = border.getResampledBySpacing( 10 );
	}
}

void Stone::renderBrushStone( std::vector< ofPoint > points ) {
	/*
	layer.begin();
	ofPushStyle();
	ofEnableAlphaBlending();

	for( int i = 0; i < points.size(); i++ ) {
		ofVec2f p = points.at( i );
		float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
		ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
		brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
	}

	ofDisableAlphaBlending();
	ofPopStyle();
	layer.end();
	*/
}

void Stone::grow()
{
	currentGrowRad += 0.5f;
	if( currentGrowRad < maxGrowRad ) {
		calcBorder( locationsPointsDrawn );

		//underlyingLayer.begin();
		//ofClear( 1.0 );
		//underlyingLayer.end();

		renderBorder();

		//layer.begin();

		int nrToCheck = ( int ) ( ofMap( currentGrowRad, 0, maxGrowRad, 5, 15 ) );

		ofPushStyle();
		ofEnableAlphaBlending();
		for( int i = 0; i < nrToCheck; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			//int randomId = ofRandom( 0, points.size() );
			
			ofVec2f p( centroid );
			//ofVec2f p = getCenterById( randomId );
			p += ofVec2f( _x, _y );

			locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
			//brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );

		}
		ofDisableAlphaBlending();
		ofPopStyle();

		//layer.end();
	}
}


void Stone::setTransparency( float _trans )
{
	transparency = _trans;
}

void Stone::setBorderTransparency( float _trans )
{
	this->borderTransparency = _trans;
}

//ofFbo Stone::getStoneBuffer()
//{
//	return layer;
//}

void Stone::setBorderSize( int _bsize )
{
	this->borderSize = _bsize;
}

int Stone::getBorderSize()
{
	return this->borderSize;
}

void Stone::setSaturation( float _sat )
{
	this->saturation = _sat;
}

float Stone::getSaturation()
{
	return this->saturation;
}

vector<ofVec3f> Stone::resamplePolylineToCount( const ofPolyline& polyline, int n )
{
	vector<ofVec3f> points;
	float jump = 1.0f / ( n - 1 );

	for( int i = 0; i < n; i++ ) {
		points.push_back( polyline.getPointAtPercent( min( i*jump, 0.99999f ) ) );
	}
	return points;
}

float Stone::getTransparency()
{
	return this->transparency;
}

void Stone::toggleRenderBorder( bool _b )
{
	this->tDrawBorder = _b;
}

void Stone::toggleDrawStone( bool _s )
{
	this->tDrawStone = _s;
}


void Stone::setFuzzy( float fuzzy )
{
	this->fuzzy = fuzzy;
}

void Stone::setRadius( float rad )
{
	this->radius = rad;
}

float Stone::getRadius()
{
	return this->radius;
}

float Stone::getFuzzy()
{
	return this->fuzzy;
}

void Stone::setSize( int size )
{
	this->size = size;
}

int Stone::getNumberOfCircles()
{
	return this->size;
}

std::vector< ofPoint > Stone::getContourPoints( float x, float y )
{
	return contourPoints;
}

int Stone::getNumberOfStrokes()
{
	return getNumberOfCircles();
}
