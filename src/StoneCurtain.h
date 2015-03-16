#pragma once

#include "ColorCollection.h"
#include "BrushCollection.h"
#include "Stone.h"

class StoneCurtain 
{
public:
	StoneCurtain();
	~StoneCurtain();

	void setBrushCollection( BrushCollection _b );
	void setColorCollection( ColorCollection _c );

	void render();
	void draw( float x, float y );

	void setTransparency( float _trans );

	std::vector< Stone > stones;

protected:
	ofFbo buffer;
	float transparency;

	BrushCollection brushes;
	ColorCollection colors;
};

