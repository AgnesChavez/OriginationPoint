#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxCv.h"

#include "BrushCollection.h"

class Stone
{
public:
	Stone();
	~Stone();
	void init( float _x, float _y );
	void draw( ofVec2f centerLocation );
	void draw( float x, float y );

	void setRadius( float rad );
	void setFuzzy( float fuzzy );
	void setSize( int size );
	float getRadius();
	float getFuzzy();
	int getNumberOfCircles();

	void setBrushCollection( BrushCollection _b );

	ofVec2f getCenterById( int id );
	float getRadiusById( int id );

	std::vector< ofVec2f > getContourPoints( float x, float y);

private:
	float radius, fuzzy;
	int size;
	
	void addCircle( ofVec2f poi, float rad );
	void calcContour(float x, float y);
	void renderStone();
	void renderBorder();
	int getNumberOfStrokes();

	std::vector< ofVec2f > points;
	std::vector< float > radii;

	ofFbo layer;
	BrushCollection brushes;


	ofFbo tempImage;
	ofxCv::ContourFinder contourFinder;
	std::vector< ofVec2f > contourPoints;
};

