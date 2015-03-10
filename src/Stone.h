#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxCv.h"

class Stone
{
public:
	Stone();
	~Stone();
	void init();
	void draw( ofVec2f centerLocation );

	void setRadius( float rad );
	void setFuzzy( float fuzzy );
	void setSize( int size );
	float getRadius();
	float getFuzzy();
	int getNumberOfCircles();

	ofVec2f getCenterById( int id );
	float getRadiusById( int id );

	std::vector< ofVec2f > getContourPoints( float x, float y);

private:
	float radius, fuzzy;
	int size;
	
	void addCircle( ofVec2f poi, float rad );
	void calcContour(float x, float y);
	std::vector< ofVec2f > points;
	std::vector< float > radii;
	std::vector< ofVec2f > contourPoints;

	ofFbo tempImage;
	ofxCv::ContourFinder contourFinder;
};

