#include "StopMotionStonesAct.h"

StopMotionStonesAct::StopMotionStonesAct()
{
	setup();
}

StopMotionStonesAct::~StopMotionStonesAct()
{

}

void StopMotionStonesAct::setup()
{
	stones.init();

	for( int i = 0; i < 10; i++ ) {
		stones.moveRandom( 6 );
	}

	stones.setGrowing( true );

	transparency = 255;

	stones.start();
}

void StopMotionStonesAct::update( unsigned long long millis )
{
	//stones.vectorFieldTransparency += 2.0f;
	//stones.vectorFieldTransparency = std::min( stones.vectorFieldTransparency, 255.0f );
	stones.transparency = transparency;
	stones.update( millis );
}

void StopMotionStonesAct::draw()
{
	stones.draw();

	
}
