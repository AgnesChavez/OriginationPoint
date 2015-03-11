#pragma once

#include "ofMain.h"

class BrushCollection
{
public:
	BrushCollection();
	~BrushCollection();

	ofImage getRandomBrush();
	ofImage getCircleBrush();
private:
	std::vector< ofImage > brushes;
	ofImage circleBrush;
};

