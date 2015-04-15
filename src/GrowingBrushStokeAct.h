#pragma once

#include "ofMain.h"
#include "StoneCurtain.h"
#include "VoronoiLayer.h"
#include "Stone.h"
#include "FourGrowingStonesLayer.h"

#include "ofxPostGlitch.h"
#include "ofxPostProcessing.h"
#include "ofxVectorField.h"

class GrowingBrushStokeAct {

public:
	GrowingBrushStokeAct();
	~GrowingBrushStokeAct();

	void setup();
	void updateScale();
	void lowerScale();
	void update();
	void updateFourStones();
	void updateSecondStone();
	void draw();
	void keyPressed( int key );
	void drawVoronoiWeb();
	void drawSecondStone();

	void createStone( ofPoint centerStone );


	ofFbo brushStoneBuffer;
	BrushStone * plainStone, * secondPlainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing * edgeDetectionPostProcessing, * secondEdgeDetectionPass;

	bool doScale;
	float scaleVal;
	float scaleNoiseVal, rotateNoiseVal;
	int growBrushIndex;

	ofFbo tintBuffer, secondTintBuffer;

	EdgePass::Ptr edgePass, secondEdgePass;
	NoiseWarpPass::Ptr noiseWarp;
	ConvolutionPass::Ptr convPass;

	VoronoiLayer voro;
	void addCustomVoronoiPoints();
	
	float transparency, secondPlainStoneTransparency;
	bool doDrawBackground;
	void updateRockYpos();
	float rockYpos;
	float backgroundTransparency;

	//EightGrowingRocks eightRocks;
	FourGrowingStonesLayer * fourRocks;

	// voronoi web
	std::vector< std::vector< ofPoint > > dottedPoints;
	VoronoiLayer voro2;
	ofFbo voroWebLayer;
	float voronoiWebTransparency;

	ofxPostGlitch myGlitch;

	ofColor bigRockColor, secondBigRockColor;

	//ofxVectorField vectorField;
};