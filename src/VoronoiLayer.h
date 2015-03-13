#pragma once

#include "ofxVoronoi2D.h"
#include "ofMain.h"

class VoronoiLayer
{
public:
	VoronoiLayer();
	~VoronoiLayer();

	void addPoint( float x, float y );
	void compute();
	void render();
	void draw( float x, float y );
	void clear();
	void setDisplayMode( int dm );
	std::vector< ofVec2f > getPoints();

	void setTransparency( float _trans );
	vector<ofVec2f> pts;

private:
	
	ofxVoronoi2D voronoi;
	
	ofMesh mesh;
	unsigned int count;
	int displayMode;
	float transparency;

	ofFbo buffer;
};

