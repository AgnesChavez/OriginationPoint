#pragma once

#include "ofMain.h"
#include "GrowingBrushStokesLayer.h"
#include "StoneCurtain.h"

#include "ofxPostProcessing.h"

class GrowingBrushStokeApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	GrowingBrushStokesLayer waterColorStone;

	ofFbo brushStoneBuffer;
	BrushStone plainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing post, postWarp, slowWarp;

	bool doJiggle;
	float noiseVal;
	int growBrushIndex;

	ofImage background;

	ofFbo tintBuffer;
	ofFbo stoneCurtainBuffer;
	int curtainX;

	StoneCurtain stoneCurtain;

	EdgePass::Ptr edgePass;
	NoiseWarpPass::Ptr noiseWarp, slowWarpPass;
	ConvolutionPass::Ptr convPass;
};