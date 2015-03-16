#pragma once

#include "ofMain.h"

class BarbWire
{
public:
	BarbWire();
	~BarbWire();

	void init();
	void draw();

	void setTransparency( float _trans );
	float getTransparency();
	void setHue( float _hue );
	float getHue();
	void setThickness( float _thick );
	float getThickness();
	void setBrightness( float _bright );
	float getBrightness();

private:
	int gridResolutionX, gridResolutionY;
	std::vector< ofPolyline > xLines, yLines;
	float transparency;
	int distance;
	float barbWireKnobSize;
	float gridDeformAmount;
	float barbWireThickness;
	int hueVal, brightness;
};

