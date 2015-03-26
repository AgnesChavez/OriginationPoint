#pragma once

#include "BrushStone.h"
#include "ColorCollection.h"
#include "BrushCollection.h"

#include "WaterColorCanvas.h"

class GrowingBrushStokesLayer
{
public:
	GrowingBrushStokesLayer();
	~GrowingBrushStokesLayer();

	void setBrushCollection( BrushCollection _b );
	void setColorCollection( ColorCollection _c );

	void add( float x, float y );
	void growAll();
	void draw( float x, float y );

	void setTransparency( float _trans );

private:
	int growRad;
	std::vector< BrushStone > stones;

	ofFbo buffer;
	float transparency;

	BrushCollection brushes;
	ColorCollection colors;

	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	WaterColorCanvas canvas;
};

