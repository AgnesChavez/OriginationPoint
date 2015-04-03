#pragma once

#include "ofMain.h"
//#include "GrowingBrushStokesLayer.h"
#include "StoneCurtain.h"
#include "VoronoiLayer.h"
#include "Stone.h"
#include "EightGrowingRocks.h"
#include "FourGrowingStonesLayer.h"

#include "ofxPostProcessing.h"

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

	//GrowingBrushStokesLayer waterColorStone;

	ofFbo brushStoneBuffer;
	BrushStone plainStone, secondPlainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing edgeDetectionPostProcessing, slowWarp;

	bool doScale;
	float scaleVal;
	float scaleNoiseVal, rotateNoiseVal;
	int growBrushIndex;

	ofImage background;

	ofFbo tintBuffer, secondTintBuffer;

	EdgePass::Ptr edgePass;
	NoiseWarpPass::Ptr noiseWarp;
	ConvolutionPass::Ptr convPass;

	VoronoiLayer voro;
	void addCustomVoronoiPoints();

	
	float transparency, secondPlainStoneTransparency;
	bool doDrawBackground;
	void updateRockYpos();
	float rockYpos;
	float backgroundTransparency;

	EightGrowingRocks eightRocks;
	FourGrowingStonesLayer fourRocks;

	// voronoi web
	std::vector< std::vector< ofPoint > > dottedPoints;
	VoronoiLayer voro2;
	ofFbo voroWebLayer;
	float voronoiWebTransparency;
};