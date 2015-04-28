#include "ActSequencer.h"
#include "Misc.h"

void ActSequencer::setup()
{
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );
	ofHideCursor();

	//TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	//TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	kinect.init();
	kinect.doBlobDetection = true;
	lastRockSampleCount = 15;
	lastRockCountSent = 1;

	sender.setup( HOST, PORT );

	
	act1 = new StopMotionStonesAct();
	act1->stones.start();

	act2 = new GrowingBrushStokeAct();
	act2->createStone( act1->stones.centered );
	act2->transparency = 0;

	act3 = new StoneCurtainLayer();

	buffer.allocate( Misc::getDefaultFboSettings() );
	buffer.begin();
	ofClear( 255, 255, 255, 0 );
	buffer.end();

	int WIDTH = 1280;
	int HEIGHT = 800;
	warper.setSourceRect( ofRectangle( 0, 0, ofGetWidth(), ofGetHeight() ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
	warper.setTopLeftCornerPosition( ofPoint( 0, 0 ) );             // this is position of the quad warp corners, centering the image on the screen.
	warper.setTopRightCornerPosition( ofPoint( WIDTH, 0 ) );        // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomLeftCornerPosition( ofPoint( 0, HEIGHT ) );      // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomRightCornerPosition( ofPoint( WIDTH, HEIGHT ) ); // this is position of the quad warp corners, centering the image on the screen.
	warper.setup();
	warper.load();

	currentMillisTimelinePosition = 0;
	lastElapsedMillis = 0;

	hasSentAct1 = hasSentAct2 = hasSentAct3 = hasSentPrevAct2 = hasSentPrevAct3 = false;

	visualTrigger = true;
	prevVisualTrigger = false;

	bigRockColor.setColor( ofColor( 255 ) );
	bigRockColor.setDuration( 0.5f );
	bigRockColor.setCurve( EASE_IN );

	fourRocksColor.setColor( ofColor( 255 ) );
	fourRocksColor.setDuration( 0.5f );
	fourRocksColor.setCurve( EASE_IN );

	curtainLeftColor.setColor( ofColor( 255 ) );
	curtainLeftColor.setDuration( 0.5f );
	curtainLeftColor.setCurve( EASE_IN );

	curtainRightColor.setColor( ofColor( 255 ) );
	curtainRightColor.setDuration( 0.5f );
	curtainRightColor.setCurve( EASE_IN );

	bigRockColorGui = ofColor( 239, 206, 27 );
	fourRockColorGui = ofColor( 255, 152, 29 );
	curtainLeftColorGui = ofColor( 238, 213, 21 );
	curtainRightColorGui = ofColor( 238, 213, 21 );

	setupGui();
}

void ActSequencer::update()
{
	kinect.update();
	sendKinectOscMessages( &kinect );

	unsigned long long difference = ofGetElapsedTimeMillis() - lastElapsedMillis;
	lastElapsedMillis = ofGetElapsedTimeMillis();

	currentMillisTimelinePosition += difference;

	//std::cout << "Current Millis: " << currentMillisTimelinePosition << std::endl;
	float factor = 1;
	
	if( visualTrigger ) {
		//act2->bigRockColor = ofColor( 239, 206, 27 );
		//act2->fourRocks->color = ofColor( 255, 152, 29 );
		//act3->leftColor = ofColor( 33, 110, 13 );
		//act3->rightColor = ofColor( 33, 110, 13 );
		//act2->secondBigRockColor = ofColor( 255, 152, 29 );
		//act1->stones.showVector = true;
		//act1->stones.vectorFieldTransparency += 2.0f;
		//act1->stones.vectorFieldTransparency = std::min( 255.0f, act1->stones.vectorFieldTransparency );

		act3->vectorFieldTransparency += 2.0f;
		act3->vectorFieldTransparency = std::min( 255.0f, act3->vectorFieldTransparency );
		

		if( !prevVisualTrigger )
		{
			// start color animations to colored
			

			//bigRockColor.animateTo( bigRockColorGui );
			//fourRocksColor.animateTo( fourRockColorGui );
			//curtainLeftColor.animateTo( curtainLeftColorGui );
			//curtainRightColor.animateTo( curtainRightColorGui );
		}

		prevVisualTrigger = true;
	}
	else {
		//act2->bigRockColor = ofColor( 255 );
		//act2->fourRocks->color = ofColor( 255 );
		//act3->leftColor = ofColor( 255 );
		//act3->rightColor = ofColor( 255 );
		//act1->stones.showVector = false;
		//act1->stones.vectorFieldTransparency -= 2.0f;
		//act1->stones.vectorFieldTransparency = std::max( 0.0f, act1->stones.vectorFieldTransparency );
		act3->vectorFieldTransparency -= 2.0f;
		act3->vectorFieldTransparency = std::max( 0.0f, act3->vectorFieldTransparency );
		//if( act3->vectorFieldTransparency < 5 )
		//{
		//	act3->showVectorField = false;
	//	}

		if( prevVisualTrigger )
		{
			// start color animations to b/w
			//bigRockColor.animateTo( ofColor( 255 ) );
			fourRocksColor.animateTo( ofColor( 255 ) );
			curtainLeftColor.animateTo( ofColor( 255 ) );
			curtainRightColor.animateTo( ofColor( 255 ) );
		}

		prevVisualTrigger = false;
	}

	float dt = 1.0f / 60.0f;
	bigRockColor.update( dt );
	fourRocksColor.update( dt );
	curtainLeftColor.update( dt );
	curtainRightColor.update( dt );

	act2->bigRockColor = bigRockColor.getCurrentColor();
	act2->fourRocks->color = fourRocksColor.getCurrentColor();
	act3->leftColor = curtainLeftColor.getCurrentColor();
	act3->rightColor = curtainRightColor.getCurrentColor();
	
	unsigned long long act2Time = 248000 * factor;
	unsigned long long act2FadeInTime = act2Time + 3000 * factor;
	unsigned long long act2UpdateStart = act2FadeInTime + 1000 * factor;
	unsigned long long act2StartScaleRock = act2UpdateStart + 210000 * factor;
	unsigned long long act2UpdateFourStonesStart = act2StartScaleRock + 55000 * factor; // 427000
	unsigned long long startStoneCurtain = act2UpdateFourStonesStart + 100000 * factor; // 537000
	unsigned long long moveFourSTones = startStoneCurtain + 30000 * factor;
	unsigned long long growNewBigRock = moveFourSTones + 15000 * factor;
	unsigned long long startMoveBigRock = growNewBigRock + 20000 * factor;
	unsigned long long fadeOut4Stones = startMoveBigRock + 50000 * factor;
	unsigned long long fadeOutBigStone = fadeOut4Stones + 20000 * factor;
	unsigned long long fadeoutBackground = fadeOutBigStone + 20000 * factor;
	unsigned long long fadeoutStoneCurtain = fadeoutBackground + 20000 * factor;
	unsigned long long fadeOutVectorField = fadeoutStoneCurtain + 5000 * factor;
	unsigned long long startOverMills = fadeOutVectorField + 15000 * factor;

	act3->showVectorField = false;

	if( currentMillisTimelinePosition > act2Time ) {
		act1->transparency -= 1.5;
		act1->stones.vectorFieldTransparency -= 1.5;
		if( act1->transparency < 0 ) {
			act1->transparency = 0;
		}
		if( act1->stones.vectorFieldTransparency < 0 ) {
			act1->stones.vectorFieldTransparency = 0;
		}
	}

	if( currentMillisTimelinePosition > act2FadeInTime && currentMillisTimelinePosition < fadeOutBigStone ) {
		act2->transparency = 250.0;
	}

	if( currentMillisTimelinePosition > act2UpdateStart ) {
		act2->update();

		if( currentMillisTimelinePosition > act2StartScaleRock  ) {
			act2->doScale = true;
		}

		if( currentMillisTimelinePosition > act2UpdateFourStonesStart ) {
			act2->noiseWarp->setEnabled( true );
			act2->noiseWarp->setAmplitude( act2->noiseWarp->getAmplitude() + 0.0002 );
			if( act2->noiseWarp->getAmplitude() >= 0.004 )
			{
				act2->noiseWarp->setAmplitude( 0.004 );
			}

			//act2->noiseWarp->setAmplitude( 0.004 );
			act2->noiseWarp->setFrequency( 0.976 );

			int padding = 10000;
			for( int i = 0; i < 4; i++ ) {
				if( currentMillisTimelinePosition > act2UpdateFourStonesStart + i * padding ) {
					act2->fourRocks->update( i );
				}
			}

			act2->fourRocks->transparency += 0.3;
			act2->fourRocks->transparency = std::min( 210.0f, act2->fourRocks->transparency );
			
		}
	}
	else {
		act1->update( currentMillisTimelinePosition );
	}

	if( currentMillisTimelinePosition > startStoneCurtain - 5000 ) {
		act3->vectorFieldTransparency += 0.2;
		act3->vectorFieldTransparency = std::min( 70.0f, act3->vectorFieldTransparency );
		//if( act3->vectorFieldTransparency > 20 )
		//{
		act3->showVectorField = true;
		//}
	}

	if( currentMillisTimelinePosition > startStoneCurtain && currentMillisTimelinePosition < fadeoutStoneCurtain + 20000 ) {
		act3->updateStoneCurtainPos();
		act3->stoneCurtainTransparency += 0.5f;
		act3->stoneCurtainTransparency = std::min( 130.0f, act3->stoneCurtainTransparency );
	}

	if( currentMillisTimelinePosition > moveFourSTones ) {
		act2->fourRocks->updateYpos();
	}

	if( currentMillisTimelinePosition > growNewBigRock && currentMillisTimelinePosition < fadeoutStoneCurtain ) {
		act2->updateSecondStone();
		act2->secondPlainStoneTransparency += 0.5f;
		act2->secondPlainStoneTransparency = std::min( act2->secondPlainStoneTransparency, 130.0f );
	}

	if( currentMillisTimelinePosition > startMoveBigRock ) {
		act2->updateRockYpos();
	}

	if( currentMillisTimelinePosition > fadeOut4Stones ) {
		
	}

	if( currentMillisTimelinePosition > fadeOutBigStone ) {
		act2->transparency -= 1.0f;
		act2->transparency = std::max( act2->transparency, 1.5f );
		act2->fourRocks->transparency -= 0.5f;
		act2->fourRocks->transparency = std::max( act2->fourRocks->transparency, 0.0f );
	}

	if( currentMillisTimelinePosition > fadeoutBackground ) {
		act2->secondPlainStoneTransparency -= 0.5f;
		act2->secondPlainStoneTransparency = std::max( act2->secondPlainStoneTransparency, 0.0f );
	}

	if( currentMillisTimelinePosition > fadeoutStoneCurtain ) {
		act3->stoneCurtainTransparency -= 1.0f;
		act3->stoneCurtainTransparency = std::max( act3->stoneCurtainTransparency, 0.0f );
	}
	
	if( currentMillisTimelinePosition > fadeOutVectorField ) {
		act3->vectorFieldTransparency -= 2.5f;
		act3->vectorFieldTransparency = std::max( 0.0f, act3->vectorFieldTransparency );
		std::cout << "fading out vector field trans:" << act3->vectorFieldTransparency << std::endl;
	}
	

	if( currentMillisTimelinePosition > startOverMills) {
		currentMillisTimelinePosition = 0;
		hasSentAct1 = hasSentAct2 = hasSentAct3 = hasSentPrevAct2 = hasSentPrevAct3 = false;

		act1->setup();
		//act1->stones.start();
		act1->stones.startedMillis = 0;

		act2->setup();
		act2->createStone( act1->stones.centered );
		act2->transparency = 0; 
		act2->noiseWarp->setAmplitude( 0.0 );

		act3->setup();
	}


	// act osc sending
	if( !hasSentAct1 ) {
		sendChapterOscMessages( 1 );
		hasSentAct1 = true;
	}

	if( currentMillisTimelinePosition > act2UpdateStart - 1000 ) {
		if( !hasSentPrevAct2 ) {
			sendPreChapterOscMessages( 2 );
			hasSentPrevAct2 = true;
		}
	}

	if( currentMillisTimelinePosition > act2UpdateStart ) {
		if( !hasSentAct2 ) {
			sendChapterOscMessages( 2 );
			hasSentAct2 = true;
		}
	}

	if( currentMillisTimelinePosition > act2UpdateFourStonesStart - 1000 ) {
		if( !hasSentPrevAct3 ) {
			sendPreChapterOscMessages( 3 );
			hasSentPrevAct3 = true;
		}
	}

	if( currentMillisTimelinePosition > act2UpdateFourStonesStart ) {
		if( !hasSentAct3 ) {
			sendChapterOscMessages( 3 );
			hasSentAct3 = true;
		}
	}

}

void ActSequencer::draw()
{
	buffer.begin();
	ofDisableSmoothing();
	ofBackground( 0 );
	if( act1->transparency > 0 ) {
		act1->draw();
	}

	if( act2->transparency > 0 ) {
		act2->draw();
		if( act2->fourRocks->transparency > 1 ) {
			act2->fourRocks->draw();
		}
	}

	if( act3->vectorFieldTransparency > 0 ) {
		act3->drawVectorField();
	}

	if( act3->stoneCurtainTransparency > 0 ) {
		act3->draw();
	}

	if( act2->secondPlainStoneTransparency > 0 ) {
		act2->drawSecondStone();
	}

	buffer.end();

	

	ofPushMatrix();
	ofMatrix4x4 mat = warper.getMatrix();
	ofMultMatrix( mat );

	buffer.draw( 0, 0, 1280, 800 );
	kinect.draw();

	ofPopMatrix();
	ofPushStyle();

	ofSetColor( ofColor::magenta );
	warper.drawQuadOutline();

	ofSetColor( ofColor::yellow );
	warper.drawCorners();

	ofPopStyle();
}

void ActSequencer::keyPressed( int key )
{
	//act1->keyPressed( key );

	switch( key ) {
	case ' ':
		warper.toggleShow();
		gui->toggleVisible();
		kinect.displayKinect = !kinect.displayKinect;
		break;
	case 't':
		//visualTrigger = !visualTrigger;
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 's':
		ofImage im;
		buffer.readToPixels( im.getPixelsRef() );
		im.saveImage( "screenshot_" + ofGetTimestampString() + ".png" );
		break;
	}
}

void ActSequencer::sendKinectOscMessages( KinectInteractionManager * kin )
{
	int stones = kin->getBlobs().size();
	lastRockCount.push_back( stones );
	while( lastRockCount.size() > (int)(lastRockSampleCount ) )
	{
		lastRockCount.erase( lastRockCount.begin() );
	}

	float average = 0.0;
	for( int i = 0; i < lastRockCount.size(); i++ )
	{
		average += lastRockCount.at( i );
	}

	average /= lastRockCount.size();

	int finalRockCount = static_cast< int > ( average );

	if( finalRockCount != lastRockCountSent ) {
		std::cout << "Detected stones: " << finalRockCount << std::endl;
		ofxOscMessage msg;
		msg.setAddress( "/stones" );
		msg.addIntArg( finalRockCount );
		sender.sendMessage( msg );
	}

	lastRockCountSent = finalRockCount;
}

void ActSequencer::sendChapterOscMessages( int actId )
{
	ofxOscMessage msg;
	msg.setAddress( "/act" );
	msg.addIntArg( actId );
	sender.sendMessage( msg );
}

void ActSequencer::sendPreChapterOscMessages( int preActId )
{
	ofxOscMessage msg;
	msg.setAddress( "/act_pre" );
	msg.addIntArg( preActId );
	sender.sendMessage( msg );
}

void ActSequencer::setupGui()
{
	gui = new ofxUISuperCanvas( "The Other", OFX_UI_FONT_MEDIUM );
	gui->addSpacer();
	gui->addFPSSlider( "FPS" );
	gui->addSpacer();
	gui->addLabel( "Kinect" );
	gui->addSlider( "KinectDistance", 0.0f, 255.0f, &kinect.kinectToStoneDistance );
	gui->addSlider( "Rock detection Samplecount", 1.0f, 200.0f, &lastRockSampleCount );
	gui->addSlider( "Blobsize Min", 0.0f, 10000.0f, &kinect.minBlobSize );
	gui->addSlider( "Blobsize Max", 0.0f, 10000.0f, &kinect.maxBlobSize );
	gui->addSpacer();
	gui->addLabel( "Act 3 - Colors" );
	gui->addSlider( "BigRock", 0.0f, 255.0f, bigRockColorGui.getHue() );
	gui->addSlider( "FourRocks", 0.0f, 255.0f, fourRockColorGui.getHue() );
	gui->addSlider( "CurtainL", 0.0f, 255.0f, curtainLeftColorGui.getHue() );
	gui->addSlider( "CurtainR", 0.0f, 255.0f, curtainRightColorGui.getHue() );
	gui->addLabel( "Act 3 - Transparencies" );
	gui->addSlider( "BigRockTransparency", 0.0f, 255.0f, &act2->transparency );
	gui->addSlider( "FourRocksTransparency", 0.0f, 255.0f, &act2->fourRocks->transparency );
	gui->addSlider( "CurtainTransparency", 0.0f, 255.0f, &act3->stoneCurtainTransparency );
	gui->addLabel( "Act 3 - Speed" );
	gui->addSlider( "CurtainSpeed", 0.0f, 50.0f, &act3->movementSpeed );
	gui->addSlider( "SecondBigRockSpeed", 0.0f, 50.0f, &act2->secondBigRockSpeed );
	gui->addSlider( "FourRockSpeed", 0.0f, 10.0f, &act2->fourRocks->speed );

	gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &ActSequencer::guiEvent );
}

void ActSequencer::guiEvent( ofxUIEventArgs &e )
{
	std::string name = e.getName();
	if( name == "BigRock" )
	{
		ofxUISlider * slider = ( ofxUISlider * )e.widget;
		float val = slider->getValue();
		float h, s, b;
		bigRockColorGui.getHsb( h, s , b );
		h = val;
		bigRockColorGui.setHsb( h, s, b );
		bigRockColor.animateTo( bigRockColorGui );
	} 
	else if( name == "FourRocks" )
	{
		ofxUISlider * slider = ( ofxUISlider * ) e.widget;
		float val = slider->getValue();
		float h, s, b;
		fourRockColorGui.getHsb( h, s, b );
		h = val;
		fourRockColorGui.setHsb( h, s, b );
		fourRocksColor.animateTo( fourRockColorGui );
	}
	else if( name == "CurtainL" )
	{
		ofxUISlider * slider = ( ofxUISlider * ) e.widget;
		float val = slider->getValue();
		float h, s, b;
		curtainLeftColorGui.getHsb( h, s, b );
		h = val;
		curtainLeftColorGui.setHsb( h, s, b );
		curtainLeftColor.animateTo( curtainLeftColorGui );
	}
	else if( name == "CurtainR" )
	{
		ofxUISlider * slider = ( ofxUISlider * ) e.widget;
		float val = slider->getValue();
		float h, s, b;
		curtainRightColorGui.getHsb( h, s, b );
		h = val;
		curtainRightColorGui.setHsb( h, s, b );
		curtainRightColor.animateTo( curtainRightColorGui );
	}
}
