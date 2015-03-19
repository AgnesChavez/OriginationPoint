#pragma once

#include "NoiseTexture.h"
#include "ofMain.h"

class NoiseFieldCutter
{
public:
	NoiseFieldCutter();
	~NoiseFieldCutter();

	void init();

	ofFbo *getCutout( NoiseTexture nt, ofFbo stoneMask );

private:
	ofShader cutoutShader;
	ofFbo buffer, underlyingBuffer;
};

