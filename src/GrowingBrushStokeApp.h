#pragma once

#include "ofMain.h"
#include "GrowingBrushStokesLayer.h"

#include "ofxPostProcessing.h"

class GrowingBrushStokeApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
	void keyPressed( int key );

	GrowingBrushStokesLayer waterColorStone;

	ofImage bgImage;
	ofFbo backgroundBuffer;

	ofFbo brushStoneBuffer;
	BrushStone plainStone;
	BrushCollection brushCollection;
	ColorCollection agnesColorSelection, blackWhiteColor;

	ofxPostProcessing post;
};