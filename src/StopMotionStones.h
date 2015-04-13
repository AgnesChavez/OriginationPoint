#pragma once

#include "VoronoiLayer.h"
#include "Stone.h"
#include "Stones.h"
#include "NoiseFieldCutter.h"
#include "NoiseTexture.h"
#include "ofMain.h"

#include "ofxPostProcessing.h"

#include <algorithm>

class StopMotionStones
{
public:
	StopMotionStones();
	~StopMotionStones();

	void init();
	void update( unsigned long long millis );
	void draw();

	void start();

	void moveRandom( float str );
	void setGrowing( bool gr );
	bool isGrowing();

	void detectedStoneCount( int count );

	float transparency;
	ofPoint centered;


private:

	bool isStarted;
	unsigned long long startedMillis;

	VoronoiLayer * voro;
	void drawCustomVoronoi();

	std::vector< Stone > stones;
	int currentStone, secondCurrentStone, thirdCurrentStone;
	ofFbo buffer;

	std::set< int > toDrawStone;

	NoiseTexture noi;
	Stones stonesTex;
	NoiseFieldCutter cutter;

	bool doGrow;
	int x, y, xSpacing, ySpacing;
	int lastMove1, lastMove2, lastMove3;

	ofPoint get2DFromIndex( int index );
	int getIndexFrom2D( ofPoint d );

	std::vector< ofPolyline > selectedLines;
	std::vector< float > transparencies;

	ofPoint getMostCenteredVoronoiCellCenter();
	int getMostCenteredVoronoiStoneIndex();

	std::vector< ofPoint > voronoiCentroids;
	float flickeringStonesRelativeTransparency;

	void removeOuterEdges();
	bool isWithinMillis( unsigned long long currentSystemMillis, unsigned long long start, unsigned long long end );
	bool isPastMillis( unsigned long long currentSystemMillis, unsigned long long mill );
	int doBrownianMotion( int currStone, int which );

	ofxPostProcessing * waterEffect;
	NoiseWarpPass::Ptr waterEffectPointer;
};

