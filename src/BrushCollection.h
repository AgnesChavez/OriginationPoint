#pragma once

#include "ofMain.h"

class BrushCollection
{
public:
	BrushCollection();
	~BrushCollection();

	ofImage getRandomBrush();
	ofImage getCircleBrush();
	ofImage getOwnBrush1();
private:
	std::vector< ofImage > brushes;
	ofImage circleBrush, ownBrush1;
};

