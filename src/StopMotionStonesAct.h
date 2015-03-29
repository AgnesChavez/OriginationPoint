#pragma once

#include "StopMotionStones.h"
#include "ofMain.h"

class StopMotionStonesAct
{
public:
	StopMotionStonesAct();
	~StopMotionStonesAct();
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	StopMotionStones stones;
	float transparency;
};