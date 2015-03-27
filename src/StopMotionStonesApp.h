#pragma once

#include "StopMotionStones.h"
#include "ofMain.h"

class StopMotionStonesApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	StopMotionStones stones;
};