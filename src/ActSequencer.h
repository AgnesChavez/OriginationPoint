#pragma once

#include "ofMain.h"

#include "StopMotionStonesAct.h"
#include "GrowingBrushStokeAct.h"

class ActSequencer : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	StopMotionStonesAct * act1;

	GrowingBrushStokeAct * act2;
	float act2Transparency;
};

