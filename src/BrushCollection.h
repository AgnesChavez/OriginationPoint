#pragma once

#include "ofMain.h"

class BrushCollection
{
public:
	BrushCollection();
	~BrushCollection();

	ofFbo getRandomBrush();
	ofImage getCircleBrush();
	ofImage getOwnBrush1();
private:
	std::vector< ofFbo > fboBrushes;
	std::vector< ofImage > brushes;
	ofImage circleBrush, ownBrush1;
};

