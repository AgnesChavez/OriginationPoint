#pragma once

#include "ColorCollection.h"
#include "BrushCollection.h"
#include "BrushStone.h"

class StoneCurtain 
{
public:
	StoneCurtain();
	~StoneCurtain();

	void setBrushCollection( BrushCollection _b );
	void setColorCollection( ColorCollection _c );
	void render();
	void grow();
	void draw( float x, float y );

	void setTransparency( float _trans );

	std::vector< BrushStone > stones;

protected:
	ofFbo buffer;
	float transparency;

	BrushCollection brushes;
	ColorCollection colors;
};

