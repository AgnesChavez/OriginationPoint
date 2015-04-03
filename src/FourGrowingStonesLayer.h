#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "ColorCollection.h"
#include "VoronoiLayer.h"
#include "ofxPostProcessing.h"

class FourGrowingStonesLayer
{
public:
	FourGrowingStonesLayer();
	~FourGrowingStonesLayer();

	void init();
	void update( int index );
	void draw();

	void updateYpos();

	VoronoiLayer voroFor4Stones;
	std::vector< BrushStone > fourStonesLayer;
	BrushCollection brushes;

	ofFbo fbo;
	ofxPostProcessing edge;
	float transparency;

	float ypos;
};

