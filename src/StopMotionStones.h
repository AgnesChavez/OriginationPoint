#pragma once

#include "VoronoiLayer.h"
#include "Stone.h"
#include "Stones.h"
#include "NoiseFieldCutter.h"
#include "NoiseTexture.h"
#include "ofMain.h"

#include <algorithm>

class StopMotionStones
{
public:
	StopMotionStones();
	~StopMotionStones();

	void init();
	void update();
	void draw();

	void moveRandom( float str );
	void setGrowing( bool gr );

private:
	VoronoiLayer * voro;
	std::vector< Stone > stones;
	int currentStone;
	ofFbo buffer;

	NoiseTexture noi;
	Stones stonesTex;
	NoiseFieldCutter cutter;

	bool doGrow;
};

