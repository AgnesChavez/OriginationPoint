#include "Stone.h"


Stone::Stone( ) :
	erodeKernel( cv::Size( 3, 3 ), CV_8UC1, cv::Scalar( 1 ) )
{
	TS_START( "stone_construc" );
	setFuzzy( 10.0f );
	setRadius( 40.0f );
	setSize( 60 );
	transparency = 255;
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

void Stone::init( float _x, float _y )
{
	TS_START( "clear" );
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();
	

	//ofSeedRandom();

	points.clear();
	radii.clear();

	TS_STOP( "clear" );
	TS_START( "circle_add" );
	ofVec2f center( _x, _y );
	addCircle( center, getRadius() );

	for( int i = 0; i < getNumberOfCircles(); i++ ) {
		center.x += ofRandomf() * fuzzy;
		center.y += ofRandomf() * fuzzy;
		addCircle( center, radius );
	}
	TS_STOP( "circle_add" );
	TS_START( "render_underl" );
	ofBackground( 0 );
	//renderUnderlying();
	TS_STOP( "render_underl" );

	
	
	TS_START( "rendering" );
	//renderStone();
	TS_START( "render_stone" );
	renderStone8();
	TS_STOP( "render_stone" );
	TS_START( "calc_cont" );
	calcContour();
	TS_STOP( "calc_cont" );

	TS_START( "render_border" );
	renderBorder();
	TS_STOP( "render_border" );
	
	TS_STOP( "rendering" );
}

void Stone::renderStone()
{
	layer.begin();
	
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	

	for( int i = 0; i < getNumberOfStrokes(); i++ )
	{
		float size = ofRandom( 20, 100 );
		ofVec2f p = getCenterById( i );
		
		ofSetColor( ofRandom( 0, 49 ), ofRandom( 0, 49 ), ofRandom( 0, 49 ), 180 );
		brushes.getRandomBrush().draw( p.x - size / 2.0, p.y - size / 2.0, size, size );

	}

	ofDisableAlphaBlending();
	ofPopStyle();

	layer.end();
}

void Stone::renderBorder()
{
	/*
	layer.begin();

	

	layer.end();

	*/

	underlyingLayer.begin();
	ofPushStyle();
	ofEnableAlphaBlending();
	//ofEnableBlendMode( OF_BLENDMODE_ADD );

	if( contourPoints.size() > 0 ) {
		ofNoFill();
		ofBeginShape();
		for( int i = 0; i < contourPoints.size(); i += 2 ) {
			ofPoint _poi = contourPoints.at( i );
			ofVertex( _poi );
		}
		ofEndShape( true );
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
	//calcContour( x, y );
	return contourPoints;
}

void Stone::calcContour()
{
	TS_START( "drawing" );
	/*
	tempFbo.begin();
	ofBackground( 0 );
	ofPushStyle();
	ofSetColor( 255 );
	for( int i = 0; i < points.size(); i++ ) {
		ofVec2f p = points.at( i );
		float rad = radii.at( i );
		ofEllipse( p.x, p.y, rad, rad );
	}
	ofPopStyle();
	tempFbo.end();
	*/
	TS_STOP( "drawing" );

	//contourFinder.setMinAreaRadius( 10 );
	//contourFinder.setMaxAreaRadius( 200 );
	contourFinder.setThreshold( 120 );

	//reader.readToPixels( tempFbo, pixels );
	TS_START( "moving_pix" );
	//underlyingLayer.readToPixels( pixels );
	//testMat= ofxCv::toCv( pixels );
	TS_STOP( "moving_pix" );
	
	//for( int i = 0; i < 1; i++ ) {
		//cv::erode( testMat, testMat, erodeKernel );
	//}

	TS_START( "point_calc" );
	//TODO: insert convex hull here
	std::cout << "points: " << locationsPointsDrawn.size() << std::endl;
	//while( locationsPointsDrawn.size() > 1000 ) {
//		std::cout << "Erasing. now: " << locationsPointsDrawn.size() << std::endl;
//		locationsPointsDrawn.erase( locationsPointsDrawn.begin() );
//	}
	contourPoints = convexHull.getConvexHull( locationsPointsDrawn );

	int hey = 0;
	/*
	contourFinder.findContours( pixels );
	contourPoints.clear();

	std::vector< cv::Point > cPoints = contourFinder.getContour( 0 );
	for( int i = 0; i < cPoints.size(); i++ ) {
		ofVec2f vecp( cPoints.at( i ).x, cPoints.at( i ).y );
		contourPoints.push_back( vecp );
	}

	*/
	TS_STOP( "point_calc" );
}

int Stone::getNumberOfStrokes()
{
	return getNumberOfCircles();
}

void Stone::setBrushCollection( BrushCollection _b )
{
	this->brushes = _b;
}

void Stone::renderStone8()
{
	layer.begin();

	ofPushStyle();
	ofEnableAlphaBlending();
	//ofEnableBlendMode( OF_BLENDMODE_ADD );

	for( int i = 0; i < brushStokeCount; i++ )
	{
		float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
		int randomId = ofRandom( 0, getNumberOfCircles() );
		ofVec2f p = getCenterById( randomId );

		ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
		locationsPointsDrawn.push_back( ofVec2f( p.x - s / 2.0, p.y - s / 2.0 ) );
		brushes.getOwnBrush1().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );

	}

	ofDisableAlphaBlending();
	ofPopStyle();

	layer.end();
}


void Stone::rerender()
{
	TS_START( "rerender" );
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();

	locationsPointsDrawn.clear();

	//renderUnderlying();

	

	//renderStone();
	renderStone8();
	calcContour();
	renderBorder();
	
	TS_STOP( "rerender" );
}

void Stone::grow()
{
	currentGrowRad += 0.2f;
	/*	
	underlyingLayer.begin();
	ofPushStyle();

	float _x = currentGrowRad * cos( ofRandom( 0, TWO_PI ) );
	float _y = currentGrowRad * sin( ofRandom( 0, TWO_PI ) );
	ofVec2f _pos = centroid + ofVec2f( _x, _y );
	ofEllipse( _pos.x, _pos.y, 7, 7 );

	ofPopStyle();
	underlyingLayer.end();
	*/
	


	calcContour();

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
		int randomId = ofRandom( 0, getNumberOfCircles() );
		ofVec2f p = getCenterById( randomId );
		p += ofVec2f( _x, _y );

		ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
		locationsPointsDrawn.push_back( ofVec2f( p.x , p.y ) );
		brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
	}
	ofDisableAlphaBlending();
	ofPopStyle();

	layer.end();
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