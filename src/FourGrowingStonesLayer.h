#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "ColorCollection.h"
#include "VoronoiLayer.h"
#include "ofxPostProcessing.h"
#include "ofxPostGlitch.h"

class FourGrowingStonesLayer
{
public:
	FourGrowingStonesLayer();
	~FourGrowingStonesLayer();

	void init();
	void update( int index );
	void draw();

	float speed;

	void updateYpos();

	VoronoiLayer voroFor4Stones;
	std::vector< BrushStone > fourStonesLayer;
	BrushCollection brushes;

	ofFbo fbo;
	ofxPostProcessing edge;
	float transparency;

	ofColor color;

	float ypos;

	//ofxPostGlitch	glitch;
};

