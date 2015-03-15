#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetVerticalSync( true );
	ofSetFrameRate( 30 );

	wrapper.openKinect();
	wrapper.openDepthStream();

	TIME_SAMPLE_SET_FRAMERATE( 30.0f );
	//TIME_SAMPLE_SET_AVERAGE_RATE( 0.01 );
	TIME_SAMPLE_SET_DRAW_LOCATION( TIME_MEASUREMENTS_TOP_RIGHT );

	stone8ColorCollection.addColor( 187, 58, 62 );
	stone8ColorCollection.addColor( 156, 121, 57 );
	stone8ColorCollection.addColor( 190, 164, 54 );
	stone8ColorCollection.addColor( 175, 169, 45 );
	stone8ColorCollection.addColor( 150, 89, 58 );
	stone8ColorCollection.addColor( 106, 72, 70 );

	bg.loadImage( "bg_b.png" );
	
	points = 20;
	reinit();
	setupGui();

	kinectToStoneDistance = 120;
	displayKinect = true;
	doGrow = false;
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );

	if( doGrow ) {
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).grow( voro.getLine( i ) );
		}
	}

	bool inside = voro.isInside( 0, ofGetMouseX(), ofGetMouseY() );
	std::cout << inside << std::endl;

	wrapper.updateDepthFrame();
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
	voro.compute();
	voro.render();
	voro.draw( 0, 0 );
	TS_STOP( "voro_draw" );

	ofPixels pix;
	pix = wrapper.grayscaleImage.getPixelsRef();
	//wrapper.grayscaleImage.threshold( 160 );
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
	case 'p':
		for( int i = 0; i < stones.size(); i++ ) {
			stones.at( i ).calcBorder();
			stones.at( i ).renderBorder();
		}
		break;
	case 'g':
		doGrow = !doGrow;
		break;
	case ' ':
		gui->toggleVisible();
		displayKinect = !displayKinect;
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
	else if( e.getName() == "Rerender" )
	{
		ofxUIButton * button = e.getButton();
		if( button->getValue() == 1 )
		{
			for( int i = 0; i < stones.size(); i++ ) {
				stones.at( i ).rerender( voro.getLine( i ) );
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
	gui->addSpacer();
	gui->addSlider( "VoroTransparency", 0.0f, 255.0, 255.0f );
	gui->addSlider( "StonesTransparency", 0.0f, 255.0, 255.0f );
	gui->addSlider( "BorderTransparency", 0.0f, 255.0f, 255.0f );
	gui->addSpacer( 2 );
	gui->addSlider( "KinectDistance", 0.0f, 255.0f, &kinectToStoneDistance );
	gui->addSlider( "Voronoi Smooth", 0.0f, 20.0f, 0.0, 200.0, 20.0 );
	
	gui->addButton( "Init", false );

	gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &testApp::guiEvent );

	gui->loadSettings( "guiSettings.xml" );
}

void testApp::reinit()
{
	TS_START( "reinit" );
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

		// TODO CHECK FOR IF NEW POINTS ARE ONLY INSIDE OF CORRESPONDING FACES
		// TODO GOOGLE HOW TO GET INDIVIDUAL FACES OF MESH
		// TODO GOOGLE HOW TO GET CELL FACES OF VORONOI DIAGRAM EASILY
		/*
		std::vector< ofMeshFace > faces = voro.getFaces();
		std::vector< ofMeshFace > correspondingFaces;
		for( int i = 0; i < faces.size(); i++ ) {
			ofMeshFace f = faces.at( i );
			for( int j = 0; j < 3; j++ ) {
				ofVec3f v = f.getVertex( j );
				if( ( int ) ( v.x ) == ( int ) ( p->x ) && ( int ) ( v.y ) == ( int ) ( p->y ) ) {
					std::cout << "equals at i" << std::endl;
					correspondingFaces.push_back( f );
					//continue;
				}
			}
		}
		int foo = 0;
		std::cout << correspondingFaces.size() << std::endl;
		*/
		stones.push_back( s );
	}
	
	TS_STOP( "reinit" );
}

