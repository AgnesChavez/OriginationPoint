#include "Stone.h"


Stone::Stone( )
{
	setFuzzy( 10.0f );
	setRadius( 40.0f );
	setSize( 60 );
	transparency = 255;
	borderTransparency = 255;
	brushStokeCount = 50;
	brushStrokeSizeMin = 20;
	brushStrokeSizeMax = 80;
	brushStrokeAlpha = 140;

	borderSize = 30;

	currentGrowRad = 10;

	layer.allocate( ofGetWidth(), ofGetHeight() );
	underlyingLayer.allocate( ofGetWidth(), ofGetHeight() );
}


Stone::~Stone()
{
}

void Stone::init( float _x, float _y, ofPolyline line  )
{
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();

	points.clear();
	radii.clear();

	ofVec2f center( _x, _y );
	addCircle( center, getRadius() );
	centroid = ofPoint( _x, _y );
	for( int i = 0; i < getNumberOfCircles(); i++ ) {
		center.x += ofRandomf() * fuzzy;
		center.y += ofRandomf() * fuzzy;
		if( line.inside( center ) ) {
			addCircle( center, radius );
		}
	}
	ofBackground( 0 );
}

void Stone::renderStone( ofPolyline line )
{
	layer.begin();
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	

	for( int i = 0; i < getNumberOfStrokes(); )
	{
		float size = ofRandom( 20, 100 );
		ofVec2f p = getCenterById( i );
		
		ofSetColor( ofRandom( 0, 49 ), ofRandom( 0, 49 ), ofRandom( 0, 49 ), 180 );
		if( line.inside( p ) ) {
			brushes.getRandomBrush().draw( p.x - size / 2.0, p.y - size / 2.0, size, size );
			i++;
		}

	}

	ofDisableAlphaBlending();
	ofPopStyle();

	layer.end();
}

void Stone::renderBorder()
{
	underlyingLayer.begin();
	ofPushStyle();
	ofEnableAlphaBlending();
	//ofEnableBlendMode( OF_BLENDMODE_ADD );

	if( contourPoints.size() > 0 ) {
		ofFill();
		
		std::vector< Point1 > convexPoints( contourPoints.size() );
#pragma omp parallel for 
		for( int i = 0; i < contourPoints.size(); i += 1 ) {
			ofPoint _poi = contourPoints.at( i );
			Point1 p;
			p.x = _poi.x;
			p.y = _poi.y;
			convexPoints.at( i ) = p;
		}

		TS_START_NIF( "convexhullcalc" );
		std::vector< Point1 > ps = VoronoiLayer::convex_hull( convexPoints );
		TS_STOP_NIF( "convexhullcalc" );

		border.clear();
		border.setClosed( true );
		
		TS_START_NIF( "point_calc" );
		for( int i = 0; i < ps.size(); i++ ) {
			Point1 from = ps.at( i );
			Point1 to;
			if( i == ps.size() - 1 ) {
				to = ps.at( 0 );
			}
			else {
				to = ps.at( i + 1 );
			}

			for( float j = 0; j < 1.0; j += 0.2 ) {
				float _x = ofLerp( from.x, to.x, j );
				float _y = ofLerp( from.y, to.y, j );
				border.addVertex( _x, _y );
				
			}
		}
		TS_STOP_NIF( "point_calc" );
		
		border.setClosed( true );
		TS_START( "resampling" );
		border = border.getResampledBySpacing( 15 );
		TS_STOP( "resampling" );

		ofSetColor( 51, 25, 0, 255 );
		float s = 10;
		ofSeedRandom( 0 );

		TS_START( "drawing_b" );
		for( int i = 0; i < border.getVertices().size(); i++ ) {
			ofPoint p = border.getVertices().at( i );
			brushes.getRandomBrush().draw( p.x - borderSize / 2.0, p.y - borderSize / 2.0, borderSize, borderSize );
		}
		TS_STOP( "drawing_b" );

		ofSeedRandom();
	}

	ofDisableAlphaBlending();
	ofPopStyle();
	underlyingLayer.end();
}

void Stone::renderUnderlying()
{
	underlyingLayer.begin();
	ofBackground( 0 );
	centroid = ofVec2f( 0, 0 );
	for( int i = 0; i < points.size(); i++ ) {
		centroid += points.at( i );
	}
	centroid /= points.size();
	ofPushStyle();
	ofDisableAlphaBlending();
	ofFill();
	ofSetColor( 255 );
	ofPushMatrix();
	for( int i = 0; i < points.size(); i++ ) {
		ofVec2f p = points.at( i );
		float rad = radii.at( i );
		ofEllipse( p.x, p.y, rad, rad );
	}
	ofPopMatrix();
	ofPopStyle();

	underlyingLayer.end();
}


void Stone::addCircle( ofVec2f poi, float rad )
{
	points.push_back( poi );
	radii.push_back( rad );
}

void Stone::draw( float x, float y )
{
	ofPushStyle();
	ofSetColor( 255, transparency );
	layer.draw( x, y );
	ofSetColor( 255, borderTransparency );
	underlyingLayer.draw( x, y );
	ofPopStyle();
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

ofVec2f Stone::getCenterById( int id )
{
	return points.at( id );
}

float Stone::getRadiusById( int id )
{
	return radii.at( id );
}

std::vector< ofPoint > Stone::getContourPoints( float x, float y )
{
	return contourPoints;
}

void Stone::calcBorder()
{
	if( locationsPointsDrawn.size() > 3 ) {
		contourPoints = convexHull.getConvexHull( locationsPointsDrawn );
	}
}

int Stone::getNumberOfStrokes()
{
	return getNumberOfCircles();
}

void Stone::setBrushCollection( BrushCollection _b )
{
	this->brushes = _b;
}

void Stone::renderStone8(ofPolyline line )
{
	layer.begin();

	ofPushStyle();
	ofEnableAlphaBlending();
	//ofEnableBlendMode( OF_BLENDMODE_ADD );

	for( int i = 0; i < brushStokeCount; )
	{
		float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
		int randomId = ofRandom( 0, points.size() );
		ofVec2f p = getCenterById( randomId );

		ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
		if( line.inside( p ) ) {
			locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
			brushes.getOwnBrush1().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
			i++;
		}
	}

	ofDisableAlphaBlending();
	ofPopStyle();

	layer.end();
}


void Stone::rerender( ofPolyline line  )
{
	TS_START( "rerender" );
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();

	locationsPointsDrawn.clear();

	currentGrowRad = 0.0f;

	//renderStone();
	calcBorder();
	renderBorder();
	renderStone8(line);
	
	
	TS_STOP( "rerender" );
}

void Stone::grow(ofPolyline line)
{
	currentGrowRad += 0.5f;
	if( currentGrowRad < 1000 ) {
		calcBorder();

		underlyingLayer.begin();
		ofClear( 1.0 );
		underlyingLayer.end();
		TS_START( "renderborder" );
		renderBorder();
		TS_STOP( "renderborder" );

		TS_START( "drawing" );
		layer.begin();

		ofPushStyle();
		ofEnableAlphaBlending();
		std::vector< ofVec2f > pointsToDraw( 5 );
//#pragma omp parallel for 
		for( int i = 0; i < 5; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			int randomId = ofRandom( 0, points.size() );
			ofVec2f p = line.getCentroid2D();
			//ofVec2f p = getCenterById( randomId );
			p += ofVec2f( _x, _y );
			pointsToDraw.at( i ) = p;
		}

		for( int i = 0; i < pointsToDraw.size(); i++ ) {
			ofVec2f p = pointsToDraw.at( i );
			float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
			ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
			if( line.inside( p ) ) {
				locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
				brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
			}
		}
		ofDisableAlphaBlending();
		ofPopStyle();

		layer.end();
		TS_STOP( "drawing" );
	}
}

void Stone::grow()
{
	currentGrowRad += 0.5f;
	if( currentGrowRad < 1000 ) {
		calcBorder();

		underlyingLayer.begin();
		ofClear( 1.0 );
		underlyingLayer.end();

		renderBorder();

		layer.begin();

		ofPushStyle();
		ofEnableAlphaBlending();
		for( int i = 0; i < 5; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
			int randomId = ofRandom( 0, points.size() );
			
			ofVec2f p( points.at( 0 ) );
			//ofVec2f p = getCenterById( randomId );
			p += ofVec2f( _x, _y );

			ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
			locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
			brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );

		}
		ofDisableAlphaBlending();
		ofPopStyle();

		layer.end();
	}
}



void Stone::setColorCollection( ColorCollection _c )
{
	this->colors = _c;
}

void Stone::setTransparency( float _trans )
{
	transparency = _trans;
}

void Stone::setBrushStrokeCount( int count )
{
	this->brushStokeCount = count;
}

void Stone::setBrushStrokeSizeMin( float min )
{
	this->brushStrokeSizeMin = min;
}

void Stone::setBrushStrokeSizeMax( float max )
{
	brushStrokeSizeMax = max;
}

void Stone::setBrushStrokeAlpha( float alpha )
{
	brushStrokeAlpha = alpha;
}

int Stone::getBrushStrokeCount()
{
	return this->brushStokeCount;
}

float Stone::getBrushStrokeSizeMin()
{
	return this->brushStrokeSizeMin;
}

float Stone::getBrushStrokeSizeMax()
{
	return this->brushStrokeSizeMax;
}

float Stone::getBrushStrokeAlpha()
{
	return this->brushStrokeAlpha;
}

void Stone::setBorderTransparency( float _trans )
{
	this->borderTransparency = _trans;
}

ofFbo Stone::getStoneBuffer()
{
	return layer;
}

void Stone::setBorderSize( int _bsize )
{
	this->borderSize = _bsize;
}

int Stone::getBorderSize()
{
	return this->borderSize;
}
