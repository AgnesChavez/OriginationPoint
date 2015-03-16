#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	wrapper.openKinect();
	wrapper.openDepthStream();

	TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	stone8ColorCollection.addColor( 236, 73, 78 );
	stone8ColorCollection.addColor( 197, 153, 72 );
	stone8ColorCollection.addColor( 211, 182, 60 );
	stone8ColorCollection.addColor( 197, 190, 51 );
	stone8ColorCollection.addColor( 202, 120, 78 );
	stone8ColorCollection.addColor( 152, 103, 100 );

	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( stone8ColorCollection );

	stoneCurtain.render();

	barbWire.init();

	bg.loadImage( "bg_b.png" );

	int x = 0;
	int y = 0;
	int w = 1920;
	int h = 1080;

	warper.setSourceRect( ofRectangle( 0, 0, ofGetWidth(), ofGetHeight() ) );              // this is the source rectangle which is the size of the image and located at ( 0, 0 )
	warper.setTopLeftCornerPosition( ofPoint( x, y ) );             // this is position of the quad warp corners, centering the image on the screen.
	warper.setTopRightCornerPosition( ofPoint( x + w, y ) );        // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomLeftCornerPosition( ofPoint( x, y + h ) );      // this is position of the quad warp corners, centering the image on the screen.
	warper.setBottomRightCornerPosition( ofPoint( x + w, y + h ) ); // this is position of the quad warp corners, centering the image on the screen.
	warper.setup();
	warper.load(); // reload last saved changes.

	points = 15;
	reinit();
	setupGui();

	kinectToStoneDistance = 120;
	displayKinect = true;
	doGrow = false;
	currentCurtainY = -1080;
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );

	if( doGrow ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro.getLine( i ) );
		}
	}
	TS_START( "kinect update" );
	wrapper.updateDepthFrame();
	TS_STOP( "kinect update" );
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();
	ofMatrix4x4 mat = warper.getMatrix();
	ofMultMatrix( mat );
	bg.draw( 0, 0 );

	currentCurtainY += 1;
	TS_START( "curtain_draw" );
	stoneCurtain.draw( 0, currentCurtainY );
	TS_STOP( "curtain_draw" );

	TS_START( "stones_draw" );
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).draw( 0, 0 );
	}
	TS_STOP( "stones_draw" );


	TS_START( "voro_all" );
	TS_START( "voro_compute" );
	//voro.compute();
	TS_STOP( "voro_compute" );
	TS_START( "voro_render" );
	voro.render();
	TS_STOP( "voro_render" );
	TS_START( "voro_draw" );
	voro.draw( 0, 0 );
	TS_STOP( "voro_draw" );
	TS_STOP( "voro_all" );

	TS_START( "kinect" );
	ofPixels pix;
	pix = wrapper.grayscaleImage.getPixelsRef();
	for( int i = 0; i < wrapper.grayscaleImage.height; i++ )
	{
		for( int j = 0; j < wrapper.grayscaleImage.width; j++ )
		{
			//wrapper.grayscaleImage.
			int editValue = pix.getColor( j, i ).r;

			if( ( editValue >= kinectToStoneDistance - 2 ) && ( editValue <= kinectToStoneDistance + 2 ) ) //check whether value is within range.
			{
				pix.setColor( j, i, ofColor( 255 ) );
				//image.at<uchar>( i, j ) = 255;
			}
			else
			{
				pix.setColor( j, i, ofColor( 0 ) );
			}
		}
	}
	wrapper.grayscaleImage.setFromPixels( pix );

	if( displayKinect ) {
		wrapper.grayscaleImage.draw( 0, 0 );
	}
	TS_STOP( "kinect" );
	
	barbWire.draw();

	ofPopMatrix();
	ofPushStyle();

	ofSetColor( ofColor::magenta );
	warper.drawQuadOutline();

	ofSetColor( ofColor::yellow );
	warper.drawCorners();

	ofSetColor( ofColor::magenta );
	warper.drawHighlightedCorner();

	ofSetColor( ofColor::red );
	warper.drawSelectedCorner();
	ofPopStyle();
}

//--------------------------------------------------------------
void testApp::keyPressed( int key ){
	int randomId = ( int ) ( ofRandom( 0, stones.size() ) );

	switch( key ) {
	case 's':
		ofSaveScreen( ofToString( ofGetTimestampString() + ".png" ) );
		break;
	case 'f':
		ofToggleFullscreen();
		break;
	case 'n':
		//stone.init( ofGetMouseX(), ofGetMouseY()  );
		//stones.at( ( int ) ( ofRandom( 0, stones.size() ) ) ).init( ofRandomWidth(), ofRandomHeight() );
		reinit();
		break;
	case 'c':
		stones.at( randomId ).rerender( voro.getLine( randomId ) );
		break;
	case 'g':
		doGrow = !doGrow;
		break;
	case ' ':
		gui->toggleVisible();
		displayKinect = !displayKinect;
		warper.toggleShow();
		break;
	}
}


//--------------------------------------------------------------
void testApp::keyReleased( int key ){

}

//--------------------------------------------------------------
void testApp::mouseMoved( int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged( int x, int y, int button ){

}

//--------------------------------------------------------------
void testApp::mousePressed( int x, int y, int button ){
}

//--------------------------------------------------------------
void testApp::mouseReleased( int x, int y, int button ){
}

//--------------------------------------------------------------
void testApp::windowResized( int w, int h ){

}

//--------------------------------------------------------------
void testApp::gotMessage( ofMessage msg ){

}

//--------------------------------------------------------------
void testApp::dragEvent( ofDragInfo dragInfo ){

}

void testApp::guiEvent( ofxUIEventArgs &e )
{
	
	if( e.getName() == "Init" )
	{
		ofxUIButton * button = e.getButton();
		if( button->getValue() == 1 )
		{
			reinit();
		}
	}
	else if( e.getName() == "Size" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setSize( slider->getValue() );
		}
	}
	else if( e.getName() == "Fuzzy" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setFuzzy( slider->getValue() );
		}
	}
	else if( e.getName() == "Radius" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setRadius( slider->getValue() );
		}
	}
	else if( e.getName() == "Transparency" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setTransparency( slider->getValue() );
		}
	}
	else if( e.getName() == "VoroTransparency" )
	{
		ofxUISlider * slider = e.getSlider();
		voro.setTransparency( slider->getValue() );
	}
	else if( e.getName() == "StonesTransparency" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setTransparency( slider->getValue() );
		}
	}
	else if( e.getName() == "BorderTransparency" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBorderTransparency( slider->getValue() );
		}
	}
	else if( e.getName() == "Voronoi Smooth" )
	{
		ofxUISlider * slider = e.getSlider();
		voro.setSmoothAmount( slider->getValue() );
	}
	else if( e.getName() == "Voronoi Thickness" ) {
		ofxUISlider * slider = e.getSlider();
		voro.setLineThickness( slider->getValue() );
	}
	else if( e.getName() == "CurtainTransparency" ) {
		ofxUISlider * slider = e.getSlider();
		stoneCurtain.setTransparency( slider->getValue() );
	} 
	else if( e.getName() == "Stone Border Size" ) {
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBorderSize( slider->getValue() );
		}
	} 
	else if( e.getName() == "Barbwire Color" ){
		ofxUISlider * slider = e.getSlider();
		barbWire.setHue( slider->getValue() );
	}
	else if( e.getName() == "Barbwire Transparency" ){
		ofxUISlider * slider = e.getSlider();
		barbWire.setTransparency( slider->getValue() );
	}
	else if( e.getName() == "Barbwire Thickness" ){
		ofxUISlider * slider = e.getSlider();
		barbWire.setThickness( slider->getValue() );
	}
}

void testApp::setupGui()
{
	Stone testStone = stones.at( 0 );
	gui = new ofxUISuperCanvas( "The Other", OFX_UI_FONT_MEDIUM );
	gui->addSpacer();
	gui->addFPSSlider( "FPS" );
	gui->addSpacer();
	
	gui->addSlider( "Points", 0.0f, 200.0f, &points );
	gui->addSlider( "Size", 0.0f, 100.0f, 35.0f, 200, 20 );
	gui->addSlider( "Fuzzy", 0.0f, 150.0f, 30.0f, 200, 20 );
	gui->addSlider( "Radius", 0.0f, 250.0f, 80.0f, 200, 20 );
	gui->addSlider( "Transparency", 0.0f, 255.0f, 255.0f, 200, 20 );
	gui->addSpacer();
	gui->addLabel( "Stone" );
	gui->addSlider( "Stone Border Size", 0.0, 60.0, testStone.getBorderSize(), 200.0, 20.0 );
	gui->addSlider( "StonesTransparency", 0.0f, 255.0, 255.0f );
	gui->addSlider( "BorderTransparency", 0.0f, 255.0f, 255.0f );
	gui->addSpacer( );
	gui->addLabel( "Kinect" );
	gui->addSlider( "KinectDistance", 0.0f, 255.0f, &kinectToStoneDistance );
	gui->addSpacer();
	gui->addLabel( "Voronoi" );
	gui->addSlider( "VoroTransparency", 0.0f, 255.0, 255.0f );
	gui->addSlider( "Voronoi Thickness", 0.0, 20.0, voro.getLineThickness(), 200.0, 20.0 );
	gui->addSlider( "Voronoi Smooth", 0.0f, 40.0f, voro.getSmoothAmount(), 200.0, 20.0 );
	gui->addSpacer();
	gui->addLabel( "Stone Curtain" );
	gui->addSlider( "CurtainTransparency", 0.0f, 255.0f, 255.0f );
	gui->addSpacer();
	gui->addLabel( "Barbwire" );
	gui->addSlider( "Barbwire Color", 0.0f, 360.0f, barbWire.getHue(), 200.0, 20.0 );
	gui->addSlider( "Barbwire Transparency", 0.0f, 255.0, barbWire.getTransparency(), 200.0, 20.0 );
	gui->addSlider( "Barbwire Thickness", 0.0f, 10.0, barbWire.getThickness(), 200.0, 20.0 );
	
	gui->addButton( "Init", false );

	gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &testApp::guiEvent );

	gui->loadSettings( "guiSettings.xml" );
}

void testApp::reinit()
{
	voro.clear();
	stones.clear();
	for( int i = 0; i < (int)(points); i++ ) {
		voro.addPoint( ofRandomWidth(), ofRandomHeight() );
	}
	
	voro.compute();

	for( int i = 0; i < voro.pts.size(); i++ ) {
		ofVec2f * p = &voro.pts.at( i );
		Stone s;
		s.setBrushCollection( brushCollection );
		s.setColorCollection( stone8ColorCollection );
		s.init( p->x, p->y, voro.getLine( i ) );

		stones.push_back( s );
	}
	
}

void testApp::exit()
{
	warper.save();
}

