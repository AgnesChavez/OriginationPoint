#pragma once

#include "ofMain.h"

#include "BrushStone.h"
#include "VoronoiLayer.h"
#include "StoneCurtain.h"
#include "RandomWalkLayer.h"

#include "ofxPostProcessing.h"
#include "ofxVectorField.h"


class StoneCurtainLayer
{
public:
	StoneCurtainLayer();
	~StoneCurtainLayer();
	
	void setup();
	void update();
	void draw();
	void drawVectorField();

	void updateStoneCurtainPos();
	void updateFourStonesPos();

	StoneCurtain stoneCurtain, stoneCurtain2;
	BrushCollection brushCollection;
	ofFbo stoneCurtainBuffer, stoneCurtainBuffer2;

	ofxPostProcessing edge, edge2;
	
	float stoneCurtainTransparency;
	float vectorFieldTransparency;
	double stoneCurtainXpos, stoneCurtainXpos2;

	float noiseVal;
	float movementSpeed;

	RandomWalkLayer randomWalkLayer;

	ofxVectorField vectorField;
	bool showVectorField;

	ofColor rightColor, leftColor;
};

