#include "TimerThread.h"


TimerThread::TimerThread()
{
	sender.setup( HOST, PORT );

	sent = false;
}


TimerThread::~TimerThread()
{
}

void TimerThread::threadedFunction()
{
	while( isThreadRunning() ) {
		unsigned long long millis = ofGetElapsedTimeMillis();
		//bool sent = false;
		if( millis > 9000 && sent == false )
		{
			sent = true;
			//sentGo1 = true;
			ofxOscMessage msg;
			msg.setAddress( "/go" );
			msg.addIntArg( 1 );
			sender.sendMessage( msg );
			std::cout << "send go1" << std::endl;
			stopThread();
		}
	}
}
