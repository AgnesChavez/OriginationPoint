#pragma once

#include <vector>
#include "ofMain.h"

class ColorCollection
{
public:
	ColorCollection();
	~ColorCollection();

	static ColorCollection getBlackAndWhiteColorCollection();

	void addColor( float r, float g, float b );
	void addColor( ofColor color );

	ofColor getRandomColor();
	ofColor getColorById( int id );

	int size();

	std::vector< ofColor > colors;
};

