#pragma once

#include "ofMain.h"

class Stones
{
public:
	Stones();
	~Stones();

	void init();
	void render( std::vector< ofPolyline > closedLines, std::vector< float > transparencies );
	ofFbo getBuffer();

private:
	ofFbo buffer;
};

