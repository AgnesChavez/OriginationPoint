#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "ColorCollection.h"
#include "VoronoiLayer.h"
#include "ofxPostProcessing.h"

class SixGrowingRocksLayer
{
public:
	SixGrowingRocksLayer();
	~SixGrowingRocksLayer();

	void init();
	void update();
	void draw();


	int rockCount;
	std::vector< BrushStone > rocks;
	BrushCollection brushes;

	VoronoiLayer voro;
	std::vector< std::vector< ofPoint > > dottedPoints;

	ofFbo fbo;
	ofxPostProcessing edge;
	float transparency;
};

