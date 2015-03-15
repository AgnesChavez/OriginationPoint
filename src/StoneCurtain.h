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

	std::vector< Stone > stones;

private:
	ofFbo buffer;

	BrushCollection brushes;
	ColorCollection colors;
};

