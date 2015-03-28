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
	bool isGrowing();

private:
	VoronoiLayer * voro, *secondVoro;
	std::vector< Stone > stones;
	int currentStone;
	ofFbo buffer;

	std::set< int > toDrawStone;

	NoiseTexture noi;
	Stones stonesTex;
	NoiseFieldCutter cutter;

	bool doGrow;
	int x, y;
	int lastMove;

	ofPoint get2DFromIndex( int index );
	int getIndexFrom2D( ofPoint d );

	void removeOuterEdges();
};

