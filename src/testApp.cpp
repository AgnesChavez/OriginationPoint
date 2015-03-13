#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel( OF_LOG_ERROR );

	TIME_SAMPLE_SET_FRAMERATE( 20.0f );
	//TIME_SAMPLE_SET_AVERAGE_RATE( 0.01 );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	//ofSetFrameRate( 200 );

	stone8ColorCollection.addColor( 187, 58, 62 );
	stone8ColorCollection.addColor( 156, 121, 57 );
	stone8ColorCollection.addColor( 190, 164, 54 );
	stone8ColorCollection.addColor( 175, 169, 45 );
	stone8ColorCollection.addColor( 150, 89, 58 );
	stone8ColorCollection.addColor( 106, 72, 70 );

	

	bg.loadImage( "bg_b.png" );
	
	buffer.allocate( ofGetWidth(), ofGetHeight() );

	points = 10;
	reinit();
	setupGui();

	doGrow = false;
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
	//voro.setTransparency( ofMap( ofGetMouseX(), 0, ofGetWidth(), 0, 255 ) );
	//stones.at( 0 ).setTransparency( ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 255 ) );

	if( doGrow ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow();
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	TS_START( "bg_draw" );
	bg.draw( 0, 0 );
	TS_STOP( "bg_end" );

	TS_START( "stones_draw" );
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).draw( 0, 0 );
	}
	TS_STOP( "stones_draw" );

	TS_START( "voro_draw" );
	voro.render();
	voro.draw( 0, 0 );
	TS_STOP( "voro_draw" );
}

//--------------------------------------------------------------
void testApp::keyPressed( int key ){
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
		stones.at( ofRandom( 0, stones.size() ) ).rerender();
		break;
	case 'p':
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).calcContour();
			stones.at( i ).renderBorder();
		}
		break;
	case 'g':
		doGrow = !doGrow;
		break;
	case ' ':
		gui->toggleVisible();
		break;
	case '1':
	case '2':
	case '3':
	case '4':
		voro.setDisplayMode( key );
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
	if( e.getName() == "Rock random Position" )
	{
		ofxUIButton * button = e.getButton();
		if( button->getValue() == 1 )
		{
			Stone s;
			s.setBrushCollection( brushCollection );
			s.setColorCollection( stone8ColorCollection );

			s.init( ofRandomWidth(), ofRandomHeight() );

			stones.push_back( s );
		}
	}
	else if( e.getName() == "Init" )
	{
		ofxUIButton * button = e.getButton();
		if( button->getValue() == 1 )
		{
			reinit();
		}
	}
	else if( e.getName() == "Rerender" )
	{
		ofxUIButton * button = e.getButton();
		if( button->getValue() == 1 )
		{
			for( int i = 0; i < stones.size(); i++ ) {
				stones.at( i ).rerender();
			}
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
	else if( e.getName() == "BrushStrokeCount" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBrushStrokeCount( slider->getValue() );
		}
	}
	else if( e.getName() == "BrushStokeSizeMin" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBrushStrokeSizeMin( slider->getValue() );
		}
	}
	else if( e.getName() == "BrushStokeSizeMax" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBrushStrokeSizeMax( slider->getValue() );
		}
	}
	else if( e.getName() == "BrushStrokeSizeAlpha" )
	{
		ofxUISlider * slider = e.getSlider();
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).setBrushStrokeAlpha( slider->getValue() );
		}
	}

}

void testApp::setupGui()
{
	Stone testStone = stones.at( 0 );
	gui = new ofxUISuperCanvas( "PROJECTING PARTICLES", OFX_UI_FONT_MEDIUM );
	gui->addSpacer();
	gui->addFPSSlider( "FPS" );
	gui->addSpacer();
	
	gui->addSlider( "BrushStrokeCount", 0.0f, 150.0f, testStone.getBrushStrokeCount(), 200, 20 );
	gui->addSlider( "BrushStokeSizeMin", 0.0f, 200.0f, testStone.getBrushStrokeSizeMin(), 200, 20 );
	gui->addSlider( "BrushStrokeSizeMax", 0.0f, 200.0f, testStone.getBrushStrokeSizeMax(), 200, 20 );
	gui->addSlider( "BrushStrokeSizeAlpha", 0.0f, 255.0f, testStone.getBrushStrokeAlpha(), 200, 20 );
	gui->addButton( "Rerender", false );
	gui->addSpacer();
	gui->addSlider( "Points", 0.0f, 200.0f, &points );
	gui->addSlider( "Size", 0.0f, 100.0f, 35.0f, 200, 20 );
	gui->addSlider( "Fuzzy", 0.0f, 150.0f, 30.0f, 200, 20 );
	gui->addSlider( "Radius", 0.0f, 250.0f, 80.0f, 200, 20 );
	gui->addSlider( "Transparency", 0.0f, 255.0f, 255.0f, 200, 20 );
	gui->addButton( "Rock random Position", false );
	
	gui->addButton( "Init", false );


	gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &testApp::guiEvent );

	gui->loadSettings( "guiSettings.xml" );
}

void testApp::reinit()
{
	TS_START( "reinit" );
	TS_START( "clearing" );
	voro.clear();
	stones.clear();
	TS_STOP( "clearing" );
	TS_START( "voro_addpoints" );
	for( int i = 0; i < (int)(points); i++ ) {
		voro.addPoint( ofRandomWidth(), ofRandomHeight() );
	}
	TS_STOP( "voro_addpoints" );
	TS_START( "stone_inits" );
	for( int i = 0; i < voro.pts.size(); i++ ) {
		ofVec2f * p = &voro.pts.at( i );
	//for( ofVec2f *p : voro.pts ) {
		Stone s;
		TS_START( "add_collections" );
		s.setBrushCollection( brushCollection );
		s.setColorCollection( stone8ColorCollection );
		TS_STOP( "add_collections" );
		TS_START( "stone_init" );
		s.init( p->x, p->y );
		TS_STOP( "stone_init" );
		TS_START( "stone_push" );
		stones.push_back( s );
		TS_STOP( "stone_push" );
	}
	TS_STOP( "stone_inits" );
	TS_START( "voro_compute" );
	voro.compute();
	TS_STOP( "voro_compute" );
	TS_STOP( "reinit" );
}

