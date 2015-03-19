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

private:
	ofShader noise;
	ofFbo noiseFbo;

	float noiseSpeed, noiseZoom;
};

