#include "Stone.h"


Stone::Stone()
{
	setFuzzy( 30.0f );
	setRadius( 80.0f );
	setSize( 35 );

	tempImage.allocate( ofGetWindowWidth(), ofGetWindowHeight() );
	init();
}


Stone::~Stone()
{
}

void Stone::init()
{
	points.clear();
	radii.clear();

	ofVec2f center( 0, 0 );
	addCircle( center, getRadius() );

	for( int i = 0; i < getNumberOfCircles(); i++ ) {
		center.x += ofRandomf() * getFuzzy();
		center.y += ofRandomf() * getFuzzy();
		addCircle( center, getRadius() );
	}

	//calcContour(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f );
}

void Stone::addCircle( ofVec2f poi, float rad )
{
	points.push_back( poi );
	radii.push_back( rad );
}

void Stone::draw( ofVec2f centerLocation )
{
	ofVec2f centroid( points.at( 0 ) );
	for( int i = 0; i < points.size(); i++ ) {
		centroid += points.at( i );
	}
	centroid /= points.size();
	ofPushStyle();
	ofFill();
	ofSetColor( 0 );
	ofPushMatrix();
	ofTranslate( centroid + centerLocation );
	for( int i = 0; i < points.size(); i++ ) {
		ofVec2f p = points.at( i );
		float rad = radii.at( i );
		ofEllipse( p.x, p.y, rad, rad );
	}
	ofPopMatrix();
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

std::vector< ofVec2f > Stone::getContourPoints( float x, float y )
{
	calcContour( x, y );
	return contourPoints;
}

void Stone::calcContour(float x, float y )
{
	tempImage.begin();
	ofBackground( 0 );
	ofPushStyle();
	ofSetColor( 255 );
	for( int i = 0; i < points.size(); i++ ) {
		ofVec2f p = points.at( i ) + ofVec2f( x, y );
		float rad = radii.at( i );
		ofEllipse( p.x, p.y, rad, rad );
	}
	ofPopStyle();
	tempImage.end();

	contourFinder.setMinAreaRadius( 10 );
	contourFinder.setMaxAreaRadius( 200 );
	contourFinder.setThreshold( 120 );
	ofPixels pixels;
	tempImage.readToPixels( pixels );
	

	contourFinder.findContours( ofxCv::toCv( pixels ) );
	if( contourFinder.getContours().size() == 1 ) {
		std::cout << "Contours: " << contourFinder.getContour( 0 ).size() << std::endl;
	}

	contourPoints.clear();

	for( cv::Point p : contourFinder.getContour( 0 ) ) {
		ofVec2f vecp( p.x, p.y );
		contourPoints.push_back( vecp );
	}
}
