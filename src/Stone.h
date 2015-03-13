#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxTimeMeasurements.h"
#include "ofxConvexHull.h"

#include "BrushCollection.h"
#include "ColorCollection.h"

class Stone
{
public:
	Stone();
	~Stone();
	void init( float _x, float _y );
	void draw( float x, float y );
	void rerender();
	void renderStone8();
	void calcContour();
	void renderStone();
	void renderBorder();

	void grow();

	cv::Mat erodeKernel;
	cv::Mat testMat;
	ofPixels pixels;


	void setRadius( float rad );
	void setFuzzy( float fuzzy );
	void setSize( int size );
	float getRadius();
	float getFuzzy();
	int getNumberOfCircles();
	void setTransparency( float _trans );

	void setBrushStrokeCount( int count );
	int getBrushStrokeCount();
	void setBrushStrokeSizeMin( float min );
	float getBrushStrokeSizeMin();
	void setBrushStrokeSizeMax( float max );
	float getBrushStrokeSizeMax();
	void setBrushStrokeAlpha( float alpha );
	float getBrushStrokeAlpha();

	


	void setBrushCollection( BrushCollection _b );
	void setColorCollection( ColorCollection _c );

	ofVec2f getCenterById( int id );
	float getRadiusById( int id );

	std::vector< ofPoint > getContourPoints( float x, float y);


private:
	float radius, fuzzy;
	int size;
	float transparency;
	ofVec2f centroid;

	float currentGrowRad;

	void addCircle( ofVec2f poi, float rad );
	
	void renderUnderlying();
	int getNumberOfStrokes();

	std::vector< ofVec2f > points;
	std::vector< float > radii;

	std::vector< ofPoint > locationsPointsDrawn;
	ofxConvexHull convexHull;

	ofFbo layer, underlyingLayer;
	BrushCollection brushes;
	ColorCollection colors;
	int brushStokeCount;
	float brushStrokeSizeMin, brushStrokeSizeMax;
	float brushStrokeAlpha;

	ofFbo tempFbo;
	ofxCv::ContourFinder contourFinder;
	std::vector< ofPoint > contourPoints;


	
};

