#pragma once

#include "ofMain.h"

class RandomWalkLayer
{
public:
	RandomWalkLayer();
	~RandomWalkLayer();

	void update();
	void draw ();

	ofFbo buffer;
	int randomWalkerCount, stepLength, walkSpeed;
	float transparency;

	float lineWeidthNoiseVal, colorNoiseVal, transparencyNoiseVal;
	std::vector< ofPoint > currentPositions, lastPositions;
};

