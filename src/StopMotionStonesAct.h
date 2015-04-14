#pragma once

#include "StopMotionStones.h"
#include "ofMain.h"


class StopMotionStonesAct
{
public:
	StopMotionStonesAct();
	~StopMotionStonesAct();
	void setup();
	void update( unsigned long long millis );
	void draw();

	StopMotionStones stones;
	float transparency;
};