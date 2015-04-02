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
	void update();
	void updateFourStones();
	void updateVoronoiWeb( int speed );
	void draw();
	void keyPressed( int key );
	void drawVoronoiWeb();

	void createStone( ofPoint centerStone );

	//GrowingBrushStokesLayer waterColorStone;

	ofFbo brushStoneBuffer;
	BrushStone plainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing edgeDetectionPostProcessing, slowWarp;

	bool doScale;
	float scaleVal;
	float scaleNoiseVal, rotateNoiseVal;
	int growBrushIndex;

	ofImage background;

	ofFbo tintBuffer;

	

	EdgePass::Ptr edgePass;
	NoiseWarpPass::Ptr noiseWarp, slowWarpPass;
	ConvolutionPass::Ptr convPass;

	VoronoiLayer voro;
	void addCustomVoronoiPoints();

	
	float transparency;

	EightGrowingRocks eightRocks;
	FourGrowingStonesLayer fourRocks;

	// voronoi web
	std::vector< std::vector< ofPoint > > dottedPoints;
	VoronoiLayer voro2;
	ofFbo voroWebLayer;
	float voronoiWebTransparency;
};