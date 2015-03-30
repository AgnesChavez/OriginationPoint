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
#include "NoiseTexture.h"

class Stone
{
public:
	Stone();
	~Stone();
	void init( float _x, float _y, ofPolyline line  );
	void clear();
	void calcBorder( std::vector< ofPoint > poi);

	void grow(ofPolyline * line, bool calcDirectly );
	void grow();
	

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


	void toggleRenderBorder( bool _b );
	void toggleDrawStone( bool _s );

	float currentGrowRad;


	std::vector< ofPoint > getContourPoints( float x, float y);

	static vector<ofVec3f> resamplePolylineToCount( const ofPolyline& polyline, int n );
	ofPolyline border;
	ofVec2f centroid;
	float maxGrowRad;

private:
	float radius, fuzzy;
	int size;
	float transparency, borderTransparency, saturation;
	
	int borderSize;

	bool tDrawBorder;
	bool tDrawStone;

	

	

	int getNumberOfStrokes();

	std::vector< ofPoint > locationsPointsDrawn;
	ofxConvexHull convexHull;

	int bufferWidth, bufferHeight;


	ofxCv::ContourFinder contourFinder;
	std::vector< ofPoint > contourPoints;
};

