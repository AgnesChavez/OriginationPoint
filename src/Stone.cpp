#include "Stone.h"


Stone::Stone( ) :
	erodeKernel( cv::Size( 3, 3 ), CV_8UC1, cv::Scalar( 1 ) )
{
	TS_START( "stone_construc" );
	setFuzzy( 10.0f );
	setRadius( 40.0f );
	setSize( 60 );
	transparency = 255;
	borderTransparency = 255;
	brushStokeCount = 50;
	brushStrokeSizeMin = 20;
	brushStrokeSizeMax = 80;
	brushStrokeAlpha = 140;

	currentGrowRad = 10;

	tempFbo.allocate( ofGetWidth(), ofGetHeight(), GL_LUMINANCE );
	layer.allocate( ofGetWidth(), ofGetHeight() );
	underlyingLayer.allocate( ofGetWidth(), ofGetHeight() );
	TS_STOP( "stone_construc" );
}


Stone::~Stone()
{
}

void Stone::init( float _x, float _y, ofPolyline line  )
{
	TS_START( "clear" );
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();

	points.clear();
	radii.clear();

	TS_STOP( "clear" );
	TS_START( "circle_add" );
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
	TS_STOP( "circle_add" );

	ofBackground( 0 );	
	
	TS_START( "rendering" );
	//renderStone();
	TS_START( "render_stone" );
	renderStone8( line );
	TS_STOP( "render_stone" );
	TS_START( "calc_cont" );
	calcBorder();
	TS_STOP( "calc_cont" );

	TS_START( "render_border" );
	renderBorder();
	TS_STOP( "render_border" );
	
	TS_STOP( "rendering" );
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
		ofNoFill();
		/*
		ofBeginShape();
		for( int i = 0; i < contourPoints.size(); i += 1 ) {
			ofPoint _poi = contourPoints.at( i );
			ofVertex( _poi );
		}
		ofEndShape( true );
		*/
		
		ofPoint lastP( contourPoints.at( 0 ) );
		ofPushStyle();
		ofFill();

		for( int i = 0; i < contourPoints.size(); i += 1 ) {
			ofVec2f from( lastP );
			ofVec2f to( contourPoints.at( i ) );
			ofLine( from, to );
			/*
			float dist = from.distance( to );
			dist /= 4;
			float step = 1.0f / dist;
			for( float j = 0.0f; j < 1.0f; j += step ) {
				ofVec2f _pos = from.getInterpolated( to, j );
				_pos += ofVec2f( ofRandom( -5, 5 ), ofRandom( -5, 5) );
				ofSetColor( 255, 130 );
				int size = ofRandom( 3, 8 );
				ofEllipse( _pos.x, _pos.y, size, size );
				//brushes.getOwnBrush1().draw(  );
			}
			*/
			lastP = ofPoint( to );
		}
		ofPopStyle();
		float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
		int randomId = ofRandom( 0, points.size() );
		ofVec2f p = getCenterById( randomId );

		ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
		locationsPointsDrawn.push_back( ofVec2f( p.x - s / 2.0, p.y - s / 2.0 ) );
		brushes.getOwnBrush1().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
		
	}

	ofDisableAlphaBlending();
	ofPopStyle();
	underlyingLayer.end();
}

void Stone::renderUnderlying()
{
	underlyingLayer.begin();
	//ofClear( 255, 255, 255 );
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
	TS_START( "stone_draw" );
	ofPushStyle();
	ofSetColor( 255, transparency );
	layer.draw( x, y );
	ofSetColor( 255, borderTransparency );
	underlyingLayer.draw( x, y );
	ofPopStyle();
	TS_STOP( "stone_draw" );
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
	contourPoints = convexHull.getConvexHull( locationsPointsDrawn );
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

	//renderUnderlying();

	

	//renderStone();
	renderStone8(line);
	calcBorder();
	renderBorder();
	
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

		//renderBorder();

		layer.begin();

		ofPushStyle();
		ofEnableAlphaBlending();
		for( int i = 0; i < 5; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
			int randomId = ofRandom( 0, points.size() );
			ofVec2f p = getCenterById( randomId );
			p += ofVec2f( _x, _y );

			ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
			if( line.inside( p ) ) {
				locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
				brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
			}
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
