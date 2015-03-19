#pragma once

#include "ofMain.h"
class NoiseTexture
{
public:
	NoiseTexture();
	~NoiseTexture();

	void render();
	void draw( int x, int y );

	void setSpeed( float _s );
	void setZoom( float _z );

	ofFbo getBuffer();

	float w1, w2, w3, w4;

private:
	ofShader noise;
	ofFbo noiseFbo;

	
	float noiseSpeed, noiseZoom;
};

