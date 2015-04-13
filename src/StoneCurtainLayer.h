#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "VoronoiLayer.h"
#include "StoneCurtain.h"

#include "ofxPostProcessing.h"

class StoneCurtainLayer
{
public:
	StoneCurtainLayer();
	~StoneCurtainLayer();
	
	void setup();
	void update();
	void draw();

	void updateStoneCurtainPos();
	void updateFourStonesPos();

	StoneCurtain stoneCurtain, stoneCurtain2;
	BrushCollection brushCollection;
	ofFbo stoneCurtainBuffer, stoneCurtainBuffer2;

	ofxPostProcessing edge, edge2;
	
	float stoneCurtainTransparency;
	double stoneCurtainXpos, stoneCurtainXpos2;

	float noiseVal;
	float movementSpeed;

};

