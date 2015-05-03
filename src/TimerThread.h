#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 5001

class TimerThread : public ofThread
{
public:
	TimerThread( );
	~TimerThread();

	ofxOscSender sender;

	bool sent;
	void threadedFunction();
};

