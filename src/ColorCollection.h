#pragma once

#include <vector>
#include "ofMain.h"

class ColorCollection
{
public:
	ColorCollection();
	~ColorCollection();

	void addColor( float r, float g, float b );
	void addColor( ofColor color );

	ofColor getRandomColor();
	ofColor getColorById( int id );

	int size();

private:
	std::vector< ofColor > colors;
};

