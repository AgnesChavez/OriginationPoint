#include "ofMain.h"
#include "testApp.h"
#include "GrowingBrushStokeApp.h"
#include "StopMotionStonesApp.h"


//========================================================================
int main(){
	ofSetWorkingDirectoryToDefault();

	ofSetupOpenGL( 1920, 1080, OF_WINDOW );			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	// ofRunApp( new testApp() );
	// act 1
	//ofRunApp( new StopMotionStonesApp() );

	// act 2
	ofRunApp( new GrowingBrushStokeApp() );
}
