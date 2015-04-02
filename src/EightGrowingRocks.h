#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "ColorCollection.h"
#include "VoronoiLayer.h"
#include "ofxPostProcessing.h"

class EightGrowingRocks
{
public:
	EightGrowingRocks();
	~EightGrowingRocks();

	void init();
	void update(int index);
	void draw();


	int rockCount;
	std::vector< BrushStone > rocks;
	BrushCollection brushes;

	VoronoiLayer voro;

	ofFbo fbo;
	ofxPostProcessing edge;
	float transparency;
};

