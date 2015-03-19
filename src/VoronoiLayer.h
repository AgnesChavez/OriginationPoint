#pragma once

#include "ofMain.h"
#include "ofxVoro.h"
#include "ofxTimeMeasurements.h"

struct Point1 {
	float x, y;

	bool operator <( const Point1 &p ) const {
		return x < p.x || ( x == p.x && y < p.y );
	}
};
	

class VoronoiLayer
{
public:
	VoronoiLayer();
	~VoronoiLayer();

	void addPoint( float x, float y );
	void addRandomPoint();
	void compute();
	void render();
	void draw( float x, float y );
	void clear();
	std::vector< ofVec2f > getPoints();
	std::vector< ofPolyline > getLines();
	ofPolyline getLine( int id );
	bool isInside( int id, float _x, float _y );
	void setLineThickness( float _thick );
	float getLineThickness();
	void toggleRender();

	void setTransparency( float _trans );
	void setSmoothAmount( int smoothA );
	int getSmoothAmount();
	vector<ofVec2f> pts;	

	static std::vector< Point1 > convex_hull( std::vector < Point1> points );
	static float cross( const Point1 &O, const Point1 &A, const Point1 &B );

private:
	voro::container * con;
	std::vector< std::vector< ofPoint > > edgePoints;

	unsigned int count;
	float transparency;
	float thickness;
	bool isDrawn;

	ofFbo buffer;
	std::vector< ofPolyline > lines;
	int smoothAmount;
};

