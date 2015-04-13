#pragma once

#include "ofMain.h"

class Misc
{
public:
	Misc();
	~Misc();

	static ofFbo::Settings getDefaultFboSettings();

	static std::vector< ofPoint > getLineSplitPoints( ofPolyline linesToSplit, float length );
	static void drawSplitLines( std::vector< ofPoint > points );
};

