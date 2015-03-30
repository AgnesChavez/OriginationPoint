#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "VoronoiLayer.h"
#include "StoneCurtain.h"

#include "ofxPostProcessing.h"

class ManyLayersAct
{
public:
	ManyLayersAct();
	~ManyLayersAct();
	
	void setup();
	void update();
	void draw();


	void updateStoneCurtainPos();
	void updateFourStonesPos();

	std::vector< BrushStone > fourStonesLayer;
	VoronoiLayer voroFor4Stones;

	ofxPostProcessing edgeDetectionPostProcessing, stoneCurtainEdgeDetectionBuffer;
	EdgePass::Ptr edgePass;

	ofxPostProcessing waterPostProcessing;
	NoiseWarpPass::Ptr waterPass;

	float transparency, stoneCurtainTransparency;

	BrushCollection brushCollection;
	ColorCollection blackWhiteColor, agnesColorSelection;

	ofImage background, gridImage;

	ofFbo stoneCurtainBuffer, fourStonesBuffer;
	StoneCurtain stoneCurtain;
	int stoneCurtainXpos;
	int fourStonesPos;

	bool doLoop4Stones;	
};

