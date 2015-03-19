#include "testApp.h"
#include "omp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 60 );
	ofSetTextureWrap( GL_REPEAT );
	
	TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	post.init( 1920, 1080 );
	nwPass = post.createPass<NoiseWarpPass>();
	nwPass->setEnabled( false );
	post.createPass<ContrastPass>()->setEnabled( true );
	post.setFlip( false );

	stone8ColorCollection.addColor( 236, 73, 78 );
	stone8ColorCollection.addColor( 197, 153, 72 );
	stone8ColorCollection.addColor( 211, 182, 60 );
	stone8ColorCollection.addColor( 197, 190, 51 );
	stone8ColorCollection.addColor( 202, 120, 78 );
	stone8ColorCollection.addColor( 152, 103, 100 );

	brightYellowColorCollection.addColor( 195, 189, 75 );
	brightYellowColorCollection.addColor( 150, 144, 70 );
	brightYellowColorCollection.addColor( 73, 78, 46 );
	brightYellowColorCollection.addColor( 201, 198, 85 );
	brightYellowColorCollection.addColor( 209, 202, 95 );

	brownColorCollection.addColor( 102, 51, 0 );
	brownColorCollection.addColor( 153, 102, 51 );
	brownColorCollection.addColor( 187, 153, 102 );

	blackWhiteColor.addColor( 255, 255, 255 );

	kinect.init();


	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( blackWhiteColor );

	//stoneCurtain.render();

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

	points = 100;
	reinit();
	setupGui();

	doGrow = false;
	currentCurtainY = -1080;


	stonesTex.init();
	cutter.init();
	for( int j = 0; j < 10; j++ ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro.getLine( i ), false, false );
		}
	}

	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).grow( voro.getLine( i ), false, true );
	}

	noi.render();

	stopmotion.init();

	ofBackground( 0 );
}

//--------------------------------------------------------------
void testApp::update(){
	currentCurtainY += 1;
	
	if( doGrow ) {
		for( int i = 0; i < stones.size(); i++ ) {
			Stone * s = &stones.at( i );
			s->grow( voro.getLine( i ), false, true );
		}
	}

	kinect.update();

	nwPass->setFrequency( 1.97 );
	nwPass->setAmplitude( 0.026 );

	stopmotion.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofPushMatrix();
	ofMatrix4x4 mat = warper.getMatrix();
	ofMultMatrix( mat );
	
	bg.draw( 0, 0 );

	voro.compute();
	voro.render();



	noi.render();
	std::vector< ofPolyline > lines;
	for( int i = 0; i < stones.size(); i++ ) {
		lines.push_back( stones.at( i ).border );
	}
	stonesTex.render( lines );
	ofFbo * buf = cutter.getCutout( noi, stonesTex.getBuffer() );
	ofPushStyle();
	ofSetColor( 255, stones.at( 0 ).getTransparency() );
	int off = 0;
	post.begin();
	buf->draw( 0, 0, 1920, 1080 );
	post.end();
	ofPopStyle();

	voro.draw( 0, 0 );
	
	//stoneCurtain.draw( 0, currentCurtainY );

	//stopmotion.draw();

	//barbWire.draw();

	//kinect.draw();

	
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
		reinit();
		break;
	case 'g':
		doGrow = !doGrow;
		break;
	case ' ':
		gui->toggleVisible();
		kinect.displayKinect = !kinect.displayKinect;
		warper.toggleShow();
		break;
	}

	unsigned idx = key - '0';
	if( idx < post.size() ) post[ idx ]->setEnabled( !post[ idx ]->getEnabled() );
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
	else if( e.getName() == "Barbwire Brightness" ){
		ofxUISlider * slider = e.getSlider();
		barbWire.setBrightness( slider->getValue() );
	}
	else if( e.getName() == "Stones Saturation" ){
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setSaturation( slider->getValue() );
		}
	}
	else if( e.getName() == "Border ON/OFF" ){
		ofxUIToggle * toggle = e.getToggle();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).toggleRenderBorder( toggle->getValue() );
		}
	}
	else if( e.getName() == "Voronoi ON/OFF" ){
		ofxUIToggle * toggle = e.getToggle();
		voro.toggleRender();
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
	gui->addSlider( "Stone Border Size", 0.0, 60.0, 0.0, 200.0, 20.0 );
	gui->addSlider( "StonesTransparency", 0.0f, 255.0, 255.0f );
	gui->addSlider( "Stones Saturation", 0.0f, 255.0, 255.0f );
	gui->addSlider( "BorderTransparency", 0.0f, 255.0f, 255.0f );
	gui->addSpacer( 3.0f );
	gui->setWidgetPosition( OFX_UI_WIDGET_POSITION_RIGHT );
	gui->addToggle( "Border ON/OFF", true );
	gui->setWidgetPosition( OFX_UI_WIDGET_POSITION_DOWN );
	
	gui->addLabel( "Kinect" );
	gui->addSlider( "KinectDistance", 0.0f, 255.0f, &kinect.kinectToStoneDistance );
	gui->addSpacer();
	gui->addLabel( "Voronoi" );
	gui->addSlider( "VoroTransparency", 0.0f, 255.0, 255.0f );
	gui->addSpacer( 3.0f );
	gui->setWidgetPosition( OFX_UI_WIDGET_POSITION_RIGHT );
	gui->addToggle( "Voronoi ON/OFF", true );
	gui->setWidgetPosition( OFX_UI_WIDGET_POSITION_DOWN );
	
	gui->addSlider( "Voronoi Thickness", 0.0, 20.0, voro.getLineThickness(), 200.0, 20.0 );
	gui->addSlider( "Voronoi Smooth", 0.0f, 40.0f, voro.getSmoothAmount(), 200.0, 20.0 );
	gui->addSpacer();
	gui->addLabel( "Stone Curtain" );
	gui->addSlider( "CurtainTransparency", 0.0f, 255.0f, 255.0f );
	gui->addSpacer();
	gui->addLabel( "Barbwire" );
	gui->addSlider( "Barbwire Color", 0.0f, 360.0f, barbWire.getHue(), 200.0, 20.0 );
	gui->addSlider( "Barbwire Transparency", 0.0f, 255.0, barbWire.getTransparency(), 200.0, 20.0 );
	gui->addSlider( "Barbwire Brightness", 0.0f, 255.0, barbWire.getBrightness(), 200.0, 20.0 );
	gui->addSlider( "Barbwire Thickness", 0.0f, 10.0, barbWire.getThickness(), 200.0, 20.0 );
	
	gui->addButton( "Init", false );
	gui->addSpacer();

	gui->addSlider( "w1", 0.0, 1.0, &noi.w1 );
	gui->addSlider( "w2", 0.0, 1.0, &noi.w2 );
	gui->addSlider( "w3", 0.0, 1.0, &noi.w3 );
	gui->addSlider( "w4", 0.0, 1.0, &noi.w4 );

	gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &testApp::guiEvent );

	gui->loadSettings( "guiSettings.xml" );
}

void testApp::reinit()
{
	
	voro.clear();
	stones.clear();
	for( int i = 0; i < (int)(points); i++ ) {
		voro.addRandomPoint();
	}
	
	voro.compute();

	
	for( int i = 0; i < voro.pts.size(); i++ ) {
		ofVec2f * p = &voro.pts.at( i );
		Stone s;
		s.init( p->x, p->y, voro.getLine( i ) );
		stones.push_back( s );
	}
	
}

void testApp::exit()
{
	warper.save();
}

