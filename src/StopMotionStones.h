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

	void start();

	void moveRandom( float str );
	void setGrowing( bool gr );
	bool isGrowing();

	float transparency;
	ofPoint centered;


private:

	bool isStarted;
	unsigned long long startedMillis;

	VoronoiLayer * voro;
	void drawCustomVoronoi();

	std::vector< Stone > stones;
	int currentStone, secondCurrentStone;
	ofFbo buffer;

	std::set< int > toDrawStone;

	NoiseTexture noi;
	Stones stonesTex;
	NoiseFieldCutter cutter;

	bool doGrow;
	int x, y, xSpacing, ySpacing;
	int lastMove;

	ofPoint get2DFromIndex( int index );
	int getIndexFrom2D( ofPoint d );

	std::vector< ofPolyline > selectedLines;
	std::vector< float > transparencies;

	ofPoint getMostCenteredVoronoiCellCenter();
	int getMostCenteredVoronoiStoneIndex();

	float currentScaleLeftOverStone;

	void removeOuterEdges();
	bool isWithinMillis( unsigned long long start, unsigned long long end );
	bool isPastMillis( unsigned long long mill );
	int doBrownianMotion( int currStone );
};

