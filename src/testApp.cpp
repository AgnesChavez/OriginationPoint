#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel( OF_LOG_ERROR );
	ofSetFrameRate( 200 );

	BrushCollection brushCollection;
	stone.setBrushCollection( brushCollection );
	stone.init( ofGetWidth() / 2.0, ofGetHeight() / 2.0 );

	setupGui();

	bg.loadImage( "bg_black.png" );
	brush.loadImage( "water_color_textures_01.png" );
	specialBrush.loadImage( "brush.png" );
	for( int i = 1; i < 26; i++ ) {
		std::stringstream ss;
		ss << "water_color_textures_";
		if( i < 10 ) {
			ss << "0";
		}
		ss << i << ".png";
		ofImage im;
		//std::string fileName = "water_color_textures_01_n.png";
		im.loadImage( ss.str() );
		brushes.push_back( im );
	}

	canvas.addPigment( ofColor( 200, 160, 90, 255 ) );
	canvas.addPigment( ofColor( 176, 75, 55, 255 ) );
	canvas.addPigment( ofColor( 54, 47, 39, 255 ) );
	canvas.addPigment( ofColor( 141, 120, 55, 255 ) );

	rockShape.allocate( ofGetWidth(), ofGetHeight() );

	state = STATE_WATER;
	currentPigment = 0;
	width = 120;
	//ofSetFrameRate( 60 );
	drawFbo = true;
	//drawBlob();
	//rockShape.readToPixels( rockPixels );

	ofPushStyle();
	randomNoiseBuffer.allocate( ofGetWidth(), ofGetHeight() );
	randomNoiseBuffer.begin();
	for( int y = 0; y < ofGetHeight(); y++ ) {
		for( int x = 0; x < ofGetWidth(); x++ ) {
			ofSetColor( ofRandom( 0, 255 ) );
			ofRect( x, y, 1, 1 );
		}
	}
	randomNoiseBuffer.end();
	ofPopStyle();
	randomNoiseBuffer.readToPixels( randomNoisePixels );

	randomBrushId = ( int ) ( ofRandom( 0, brushes.size() ) );
	applyPointillistic = false;

	brushIndex = 0;

	psBlend.setup( ofGetWidth(), ofGetHeight() );
	//bg.draw( 0, 0 );

	buffer.allocate( ofGetWidth(), ofGetHeight() );
}

//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle( ofToString( ofGetFrameRate() ) );
	
	//psBlend.begin();
	//canvas.update();
	//psBlend.end();
}

//--------------------------------------------------------------
void testApp::draw(){
	/*
	canvas.draw();

	glBlendFunc(GL_ZERO, GL_SRC_COLOR);
	bg.draw(0,0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	string str;
	ofColor currentColor = canvas.pigments.at( currentPigment).color;
	str = "color: ";
	str += ofToString((float)(currentColor.r));
	str += " ";
	str += ofToString((float)(currentColor.g));
	str += " ";
	str += ofToString((float)(currentColor.b));

	ofDrawBitmapString(str, 20, 20);
	if( drawFbo ) {
	rockShape.draw(0, 0 );
	}
	//glBlendFunc(GL_ZERO, GL_SRC_COLOR);

	if( applyPointillistic ) {
	ofPushStyle();
	std::cout << "pointilistic" << std::endl;
	for( int y = 0; y < ofGetHeight(); y++ ) {
	for( int x = 0; x < ofGetWidth(); x++ ) {
	ofColor col = randomNoisePixels.getColor( x, y );
	ofSetColor( 255, 255, 255, (float)(col.r) / 10.0f );
	ofRect( x, y, 1, 1 );

	}
	}
	drawBlob();
	ofPopStyle();
	}
	*/

	//canvas.draw();

	//glBlendFunc( GL_ZERO, GL_SRC_COLOR );
	//ofBackground( 0 );
	//bg.draw( 0, 0 );
	//psBlend.draw( canvas.waterColorCanvas.getTextureReference(), blendMode );
	//glBlendFunc( GL_ONE, GL_ONE);

	//rockShape.draw( 0, 0 );
	bg.draw( 0, 0 );
	stone.draw( 0, 0 );
	//ofEnableAlphaBlending();
	//ofEnableBlendMode( OF_BLENDMODE_ADD );
	//buffer.draw( 0, 0 );
	//ofDisableBlendMode();
}

//--------------------------------------------------------------
void testApp::strokeDraw() {
	int counter = 0;
	ofPoint target = ofPoint( mouseX, mouseY );
	float len = pos.distance( target );


	while( counter < 1000 ) {
		counter++;
		currentWidth = min( width, max( width / 4.f, currentWidth - ( len - 25 ) * 0.001f ) );

		vec += ( target - pos ).normalize() * 0.03;
		vec *= 0.97;
		if( vec.length() > 2 ) {
			vec = vec.normalize() * 2;
		}

		pos += vec;

		if( state == STATE_WATER || state == STATE_MIX ) {
			canvas.beginWaterDraw();
			ofPushStyle();
			ofSetColor( 128, ofRandom( 100, 120 ), 0, 300 / currentWidth );

			getRandomBrush().draw( pos.x - currentWidth * 1.05 / 2 * 2, pos.y - currentWidth * 1.05 / 2 * 2, currentWidth * 1.05 * 2, currentWidth * 1.05 * 2 );
			ofPopStyle();
			canvas.endWaterDraw();
		}


		if( state == STATE_PIGMENT || state == STATE_MIX ) {
			currentPigment = ( int ) ( ofRandom( 0, canvas.pigments.size() ) );
			canvas.beginPigmentDraw( currentPigment );
			ofPushStyle();
			ofSetColor( ofRandom( 100, 150 ), 0, 0, 300 / currentWidth / ( ( state == 2 ) ? 2 : 1 ) );
			getRandomBrush().draw( pos.x - currentWidth / 2 * 2, pos.y - currentWidth / 2 * 2, currentWidth * 2, currentWidth * 2 );
			ofPopStyle();
			canvas.endPigmentDraw();
		}

		if( pos.distance( target ) < 10 ) { break; }
	}

}

ofImage testApp::getRandomBrush() {
	return brushes.at( randomBrushId );
}

//--------------------------------------------------------------
void testApp::keyPressed( int key ){
	switch( key ) {
	case 'd':
		drawRockOnSurface();
		break;
	case 'w':
		drawWater();
		break;
	case 's':
		ofSaveScreen( ofToString( ofGetTimestampString() + ".png" ) );
		break;
	case 'b':
		drawStoneUnderlyingShape();
		break;
	case 'f':
		drawFbo = !drawFbo;
		break;
	case 'p':
		applyPointillistic = !applyPointillistic;
		break;
	case 'n':
		stone.init( ofGetMouseX(), ofGetMouseY()  );
		break;
	case 'z':
		drawRockOnSurfaceMoreColor( ofGetMouseX(), ofGetMouseY() );
		break;
	case 'u':
		brushIndex++;
		brushIndex %= brushes.size();
		break;
	case ' ':
		gui->toggleVisible();
		break;
	case 'i':
		drawIndependently();
		break;

	}

	if( key == OF_KEY_UP )
	{
		if( blendMode >= 24 )
		{
			blendMode = 0;
		}
		else {
			blendMode++;
		}
	}
	if( key == OF_KEY_DOWN )
	{
		if( blendMode <= 0 )
		{
			blendMode = 24;
		}
		else
		{
			blendMode--;
		}
	}
}

void testApp::drawStoneUnderlyingShape() {
	stone.draw( ofVec2f( ofGetMouseX(), ofGetMouseY() ) );
}

void testApp::drawWater() {
	state = STATE_WATER;
	float currentWidth = 150;

	for( int i = 0; i < 100; i++ ) {
		currentPigment = ( int ) ( ofRandom( 0, canvas.pigments.size() ) );

		int selectedIndex = ( int ) ( ofRandom( 0, stone.getNumberOfCircles() ) );
		ofVec2f selectedCenter = stone.getCenterById( selectedIndex );

		selectedCenter.x += ofGetMouseX();
		selectedCenter.y += ofGetMouseY();

		canvas.beginWaterDraw();
		ofPushStyle();
		ofSetColor( 128, ofRandom( 100, 120 ), 0, 300 / currentWidth );
		getRandomBrush().draw( selectedCenter.x - currentWidth * 1.05 / 2 * 2, selectedCenter.y - currentWidth * 1.05 / 2 * 2, currentWidth * 1.05 * 2, currentWidth * 1.05 * 2 );
		ofPopStyle();
		canvas.endWaterDraw();

	}

	/*
	int jump = 10;
	for( int i = 0; i < 600; i++ ) {
	ofPoint target = ofPoint( ofRandomWidth(), ofRandomHeight() );
	ofColor pixelColor = rockPixels.getColor( target.x, target.y );
	if( pixelColor.b > 120 ) {
	if (state == STATE_WATER || state == STATE_MIX) {

	}
	}

	}

	*/
}

void testApp::drawRockOnSurface() {
	float currentWidth = ofRandom( 50, 120 );
	randomBrushId = ( int ) ( ofRandom( 0, brushes.size() ) );

	state = STATE_PIGMENT;
	for( int i = 0; i < 1000; i++ ) {
		if( i % 100 == 0 ) {
			currentPigment = ( int ) ( ofRandom( 0, canvas.pigments.size() ) );
		}

		int selectedIndex = ( int ) ( ofRandom( 0, stone.getNumberOfCircles() ) );
		ofVec2f selectedCenter = stone.getCenterById( selectedIndex );
		float selectedMaxRadius = stone.getRadiusById( selectedIndex );

		// float actuallySelectedRadius = ofRandom( 0, selectedMaxRadius / 2.0f );
		float actuallySelectedRadius = 0;

		float randomAngle = ofRandom( 0, TWO_PI );

		float x = actuallySelectedRadius * cos( randomAngle );
		float y = actuallySelectedRadius * sin( randomAngle );

		x += selectedCenter.x + ofGetMouseX();
		y += selectedCenter.y + ofGetMouseY();

		if( state == STATE_PIGMENT || state == STATE_MIX ) {
			//currentPigment = (int)(ofRandom( 0,  canvas.pigments.size() ));
			canvas.beginPigmentDraw( currentPigment );
			ofPushStyle();
			ofSetColor( ofRandom( 100, 150 ), 0, 0, 300 / currentWidth / ( ( state == 2 ) ? 2 : 1 ) );
			specialBrush.draw( x - currentWidth / 2 * 2, y - currentWidth / 2 * 2, currentWidth * 2, currentWidth * 2 );
			ofPopStyle();
			canvas.endPigmentDraw();
		}
	}
}

void testApp::drawIndependently() {
	buffer.begin();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_ADD );
	//ofClear( 0.0f, 0.0f, 0.0f );
	float alpha = ofMap( ofGetMouseX(), 0, ofGetWidth(), 0, 255 );
	std::vector< ofColor > colors;
	ofPushStyle();
	colors.push_back( ofColor( 200, 160, 90, alpha ) );
	colors.push_back( ofColor( 176, 75, 55, alpha ) );
	colors.push_back( ofColor( 54, 47, 39, alpha ) );
	colors.push_back( ofColor( 141, 120, 55, alpha ) );
	for( int i = 0; i < stone.getNumberOfCircles(); i++ )
	{
		float size = ofRandom( 20, 150 );
		ofVec2f p = stone.getCenterById( i );
		p.x += ofGetMouseX();
		p.y += ofGetMouseY();
		ofSetColor( colors.at( ofRandom( 0, colors.size() ) ) );
		int brushIndex = ofRandom( 0, brushes.size() );
		brushes.at(brushIndex).draw( p.x - size / 2.0, p.y - size / 2, size, size );
	}
	
	ofDisableAlphaBlending();
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_SUBTRACT );
	float currentWidth = 10;
	state = STATE_PIGMENT;
	randomBrushId = ( int ) ( ofRandom( 0, brushes.size() ) );

	std::vector< ofVec2f > contourPoints = stone.getContourPoints( ofGetMouseX(), ofGetMouseY() );
	int i = 0;
	std::cout << "ContourPoints size: " << contourPoints.size() << std::endl;
	for( int i = 0; i < contourPoints.size(); i++ ) {
		ofVec2f contourPoint = contourPoints.at( i );
		//for( ofVec2f contourPoint : stone.getContourPoints( ofGetMouseX(), ofGetMouseY() ) ) {
		float x = contourPoint.x;
		float y = contourPoint.y;

		//ofPushStyle();
		ofSetColor( 0 );
		specialBrush.draw( x - currentWidth / 2, y - currentWidth / 2, currentWidth, currentWidth );
		//ofPopStyle();
	}
	ofPopStyle();
	ofDisableAlphaBlending();
	buffer.end();
}

void testApp::drawRockOnSurfaceMoreColor( float _x, float _y)
{
	// drawing contour

	float currentWidth = 10;
	state = STATE_PIGMENT;
	randomBrushId = ( int ) ( ofRandom( 0, brushes.size() ) );
	std::vector< ofVec2f > contourPoints = stone.getContourPoints( _x, _y );
	std::cout << "ContourPoints size: " << contourPoints.size() << std::endl;
	for( int i = 0; i < contourPoints.size(); i++ ) {
		ofVec2f contourPoint = contourPoints.at( i );
	//for( ofVec2f contourPoint : stone.getContourPoints( ofGetMouseX(), ofGetMouseY() ) ) {
		float x = contourPoint.x;
		float y = contourPoint.y;

		canvas.beginPigmentDraw( 2 );
		ofPushStyle();
		ofSetColor( ofRandom( 100, 150 ), 0, 0, 300 / currentWidth / ( ( state == 2 ) ? 2 : 1 ) );
		specialBrush.draw( x - currentWidth / 2 * 2, y - currentWidth / 2 * 2, currentWidth * 2, currentWidth * 2 );
		ofPopStyle();
		canvas.endPigmentDraw();
	}

	// drawing inside of stone

	currentWidth = ofRandom( 50, 120 );

	state = STATE_PIGMENT;
	for( int i = 0; i < stone.getNumberOfCircles(); i++ ) {
		ofVec2f p = stone.getCenterById( i );
		float x = p.x + _x;
		float y = p.y + _y;
		currentPigment = ( int ) ( ofRandom( 0, canvas.pigments.size() ) );
		randomBrushId = ( int ) ( ofRandom( 0, brushes.size() ) );
		currentWidth = brushes.at( randomBrushId ).width;
		for( int j = 0; j < 30; j++ ) {
			currentWidth = ofRandom( 20, 80 );
			canvas.beginPigmentDraw( currentPigment );
			ofPushStyle();
			ofSetColor( ofRandom( 100, 150 ), 0, 0, 300 / currentWidth / ( ( state == 2 ) ? 2 : 1 ) );
			specialBrush.draw( x - currentWidth / 2 * 2, y - currentWidth / 2 * 2, currentWidth * 2, currentWidth * 2 );
			ofPopStyle();
			canvas.endPigmentDraw();
		}
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
	pressed = true;
	pos.set( mouseX, mouseY );
	vec.set( 0, 0 );
	currentWidth = width;
}

//--------------------------------------------------------------
void testApp::mouseReleased( int x, int y, int button ){
	pressed = false;
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
			drawRockOnSurfaceMoreColor( ofRandomWidth(), ofRandomHeight() );
		}
		stone.init( ofGetMouseX(), ofGetMouseY()  );
	}
	else if( e.getName() == "Size" )
	{
		ofxUISlider * button = e.getSlider();
		stone.setSize( button->getValue() );
		stone.init( ofGetMouseX(), ofGetMouseY()  );
	}
	else if( e.getName() == "Fuzzy" )
	{
		ofxUISlider * button = e.getSlider();
		stone.setFuzzy( button->getValue() );
		stone.init( ofGetMouseX(), ofGetMouseY() );
	}
	else if( e.getName() == "Radius" )
	{
		ofxUISlider * button = e.getSlider();
		stone.setRadius( button->getValue() );
		stone.init( ofGetMouseX(), ofGetMouseY()  );
	}
	std::cout << e.getName() << std::endl;
}

void testApp::setupGui()
{
	gui = new ofxUISuperCanvas( "PROJECTING PARTICLES", OFX_UI_FONT_MEDIUM );
	gui->addSpacer();
	gui->addFPSSlider( "FPS" );
	gui->addSpacer();
	gui->addSlider( "Size", 0.0f, 100.0f, 35.0f, 100, 20 );
	gui->addSlider( "Fuzzy", 0.0f, 150.0f, 30.0f, 100, 20 );
	gui->addSlider( "Radius", 0.0f, 250.0f, 80.0f, 100, 20 );
	gui->addButton( "Rock random Position", false );

	//gui->autoSizeToFitWidgets();
	ofAddListener( gui->newGUIEvent, this, &testApp::guiEvent );

	gui->loadSettings( "guiSettings.xml" );
}

