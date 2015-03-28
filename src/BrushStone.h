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


class BrushStone
{
public:
	BrushStone();
	~BrushStone();
	void init( float _x, float _y );
	void draw( float x, float y, float w, float h );
	void clear();
	void calcBorder( std::vector< ofPoint > poi );
	void renderBorder();

	void grow( ofPolyline line );
	void grow();
	bool growForWaterColor( float rad );
	void growPlain( int brushId );


	void setRadius( float rad );
	void setFuzzy( float fuzzy );
	void setSize( int size );
	float getRadius();
	float getFuzzy();
	int getNumberOfCircles();
	void setTransparency( float _trans );
	float getTransparency();
	void setBorderTransparency( float _trans );
	void setBorderSize( int _bsize );
	int getBorderSize();
	void setSaturation( float _sat );
	float getSaturation();

	ofFbo getStoneBuffer();

	void setBrushStrokeCount( int count );
	int getBrushStrokeCount();
	void setBrushStrokeSizeMin( float min );
	float getBrushStrokeSizeMin();
	void setBrushStrokeSizeMax( float max );
	float getBrushStrokeSizeMax();
	void setBrushStrokeAlpha( float alpha );
	float getBrushStrokeAlpha();

	void setSelectedColor( ofColor col );

	void setBrushCollection( BrushCollection _b );
	void setColorCollection( ColorCollection _c );

	void toggleRenderBorder( bool _b );
	void toggleDrawStone( bool _s );


	std::vector< ofPoint > getContourPoints( float x, float y );

	static vector<ofVec3f> resamplePolylineToCount( const ofPolyline& polyline, int n );
	float currentGrowRad;

private:
	float radius, fuzzy;
	int size;
	float transparency, borderTransparency, saturation;
	ofVec2f centroid;
	int borderSize;

	bool tDrawBorder;
	bool tDrawStone;
	ofColor selectedColor;

	ofPolyline border;

	
	float maxGrowRad;

	int getNumberOfStrokes();

	std::vector< ofPoint > locationsPointsDrawn;
	ofxConvexHull convexHull;

	int bufferWidth, bufferHeight;
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
