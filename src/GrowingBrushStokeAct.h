#pragma once

#include "ofMain.h"
#include "GrowingBrushStokesLayer.h"
#include "StoneCurtain.h"
#include "VoronoiLayer.h"
#include "Stone.h"

#include "ofxPostProcessing.h"

class GrowingBrushStokeAct {

public:
	GrowingBrushStokeAct();
	~GrowingBrushStokeAct();

	void setup();
	void updateJiggle();
	void update();
	void updateFourStones();
	void draw();
	void keyPressed( int key );

	void createStone( ofPoint centerStone );

	GrowingBrushStokesLayer waterColorStone;

	ofFbo brushStoneBuffer;
	BrushStone plainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing edgeDetectionPostProcessing, slowWarp;

	bool doJiggle;
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
};