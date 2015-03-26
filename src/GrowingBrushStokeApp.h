#pragma once

#include "ofMain.h"
#include "GrowingBrushStokesLayer.h"

class GrowingBrushStokeApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	GrowingBrushStokesLayer waterColorStone;

	ofFbo brushStoneBuffer;
	BrushStone plainStone;
	BrushCollection brushCollection;
	ColorCollection stone8ColorCollection, brightYellowColorCollection, brownColorCollection, blackWhiteColor;
};