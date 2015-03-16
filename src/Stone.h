#pragma once

#include <vector>
#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxTimeMeasurements.h"
#include "ofxConvexHull.h"

#include "BrushCollection.h"
#include "ColorCollection.h"
#include "VoronoiLayer.h"


class Stone
{
public:
	Stone();
	~Stone();
	void init( float _x, float _y, ofPolyline line  );
	void draw( float x, float y );
	void rerender(ofPolyline line );
	void renderStone8(ofPolyline line );
	void calcBorder();
	void renderStone(ofPolyline line );
	void renderBorder();

	void grow(ofPolyline line );
	void grow();

	cv::Mat testMat;
	ofPixels pixels;


	void setRadius( float rad );
	void setFuzzy( float fuzzy );
	void setSize( int size );
	float getRadius();
	float getFuzzy();
	int getNumberOfCircles();
	void setTransparency( float _trans );
	void setBorderTransparency( float _trans );
	void setBorderSize( int _bsize );
	int getBorderSize();

	ofFbo getStoneBuffer();

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
	float transparency, borderTransparency;
	ofVec2f centroid;
	int borderSize;

	ofPolyline border;

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

