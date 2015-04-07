#include "GrowingBrushStokeAct.h"
#include "ActSequencer.h"

GrowingBrushStokeAct::GrowingBrushStokeAct()
{
	for( int i = 0; i < 80; i++ ) {
		std::vector< ofPoint > emptyPoints;
		dottedPoints.push_back( emptyPoints );
	}

	background.loadImage( "lines_3_bw.jpg" );
	whiteLinesBackground.loadImage( "black_room-white_lines.jpg" );

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	tintBuffer.allocate( settings );
	secondTintBuffer.allocate( settings );

	//voroWebLayer.allocate( settings );

	plainStone = new BrushStone();
	secondPlainStone = new BrushStone();
	edgeDetectionPostProcessing = new ofxPostProcessing();
	secondEdgeDetectionPass = new ofxPostProcessing();
	fourRocks = new FourGrowingStonesLayer();

	setup();
}

GrowingBrushStokeAct::~GrowingBrushStokeAct()
{
	delete plainStone;
	delete secondPlainStone;
	delete edgeDetectionPostProcessing;
	delete secondEdgeDetectionPass;
	delete fourRocks;
}


void GrowingBrushStokeAct::setup() {
	transparency = 255;
	voronoiWebTransparency = 0;
	secondPlainStoneTransparency = 0;
	voro2.clear();
	for( int i = 0; i < dottedPoints.size(); i++ ) {
		dottedPoints.at( i ).clear();
	}

	for( int i = 0; i < dottedPoints.size(); i++ ) {
		voro2.addPoint( ofRandom( 0, 1920 ), ofRandom( 0, 1080 ) );
	}
	voro2.setSmoothAmount( 15 );
	voro2.compute();
	voro2.render();

	for( int i = 0; i < voro2.getLines().size(); i++ ) {
		ofPolyline lineToDraw = *voro2.getLine( i ); // rocks.at( i ).border
		std::vector< ofPoint > pointsToDraw = ActSequencer::getLineSplitPoints( lineToDraw, 4 );
		dottedPoints.at( i ) = pointsToDraw;
	}

	doDrawBackground = false;

	doScale = false;
	scaleNoiseVal = 0.0f;
	rotateNoiseVal = 0.0f;
	growBrushIndex = 0;
	scaleVal = 0.0;
	rockYpos = 0.0;
	backgroundTransparency = 255;
	
	//ofBackground( 0 );
	blackWhiteColor.colors.clear();
	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	
	//voroWebLayer.begin();
	//drawVoronoiWeb();
	//voroWebLayer.end();

	tintBuffer.begin();
	ofBackground( 0 );
	tintBuffer.end();

	secondTintBuffer.begin();
	ofBackground( 0 );
	secondTintBuffer.end();

	delete edgeDetectionPostProcessing;
	delete secondEdgeDetectionPass;

	edgeDetectionPostProcessing = new ofxPostProcessing();
	edgeDetectionPostProcessing->init( 1920, 1080 );
	edgePass = edgeDetectionPostProcessing->createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing->setFlip( false );

	secondEdgeDetectionPass = new ofxPostProcessing();
	secondEdgeDetectionPass->init( 1920, 1080 );
	secondEdgePass = secondEdgeDetectionPass->createPass<EdgePass>();
	secondEdgePass->setEnabled( true );
	secondEdgeDetectionPass->setFlip( false );

	edgeDetectionPostProcessing->begin();
	ofBackground( 0 );
	edgeDetectionPostProcessing->end();

	secondEdgeDetectionPass->begin();
	ofBackground( 0 );
	secondEdgeDetectionPass->end();

	delete fourRocks;
	fourRocks = new FourGrowingStonesLayer();

	//plainStone.clear();
	//secondPlainStone.clear();

	addCustomVoronoiPoints();
}

void GrowingBrushStokeAct::createStone( ofPoint centerStone )
{
	delete plainStone;
	delete secondPlainStone;

	plainStone = new BrushStone();
	plainStone->setColorCollection( blackWhiteColor );
	plainStone->setBrushCollection( brushCollection );
	plainStone->setBrushStrokeAlpha( 255 );
	plainStone->init( centerStone.x, centerStone.y );
	plainStone->setBrushStrokeSizeMin( 10 );
	plainStone->setBrushStrokeSizeMax( 40 );

	for( int i = 0; i < 3; i++ ) {
		plainStone->grow( *voro.getLine( 0 ), ofVec2f( 1920 / 2, 1080 / 2 ) );
	}

	plainStone->setBrushStrokeSizeMin( 20 );
	plainStone->setBrushStrokeSizeMax( 80 );


	secondPlainStone = new BrushStone();
	secondPlainStone->setColorCollection( blackWhiteColor );
	secondPlainStone->setBrushCollection( brushCollection );
	secondPlainStone->setBrushStrokeAlpha( 255 );
	secondPlainStone->init( centerStone.x, centerStone.y );
	secondPlainStone->setBrushStrokeSizeMin( 10 );
	secondPlainStone->setBrushStrokeSizeMax( 40 );

	for( int i = 0; i < 3; i++ ) {
		secondPlainStone->grow( *voro.getLine( 0 ), ofVec2f( 1920 / 2, 1080 / 2 ) );
	}

	secondPlainStone->setBrushStrokeSizeMin( 20 );
	secondPlainStone->setBrushStrokeSizeMax( 80 );
}

void GrowingBrushStokeAct::addCustomVoronoiPoints() {

	voro.clear();

	voro.addPoint( 1920 / 2, 1080 / 2 );
	voro.addPoint( ofRandom( 100, 300 ), 100 );
	voro.addPoint( ofRandom( 300, 700 ), 100 );
	voro.addPoint( ofRandom( 700, 1200 ), 100 );
	voro.addPoint( ofRandom( 1200, 1600 ), 100 );
	voro.addPoint( ofRandom( 1600, 1920 ), 100 );

	voro.addPoint( ofRandom( 100, 300 ), 1000 );
	voro.addPoint( ofRandom( 300, 700 ), 1000 );
	voro.addPoint( ofRandom( 700, 1200 ), 1000 );
	voro.addPoint( ofRandom( 1200, 1600 ), 1000 );
	voro.addPoint( ofRandom( 1600, 1920 ), 1000 );

	voro.addPoint( 100, ofRandom( 100, 300 ) );
	voro.addPoint( 100, ofRandom( 300, 800 ) );
	voro.addPoint( 100, ofRandom( 800, 1100 ) );

	voro.addPoint( 1800, ofRandom( 100, 300 ) );
	voro.addPoint( 1800, ofRandom( 300, 800 ) );
	voro.addPoint( 1800, ofRandom( 800, 1100 ) );

	voro.setSmoothAmount( 15 );

	voro.compute();
	voro.render();
}


void GrowingBrushStokeAct::update() {
	if( ofGetFrameNum() % 6 == 0 ) {
		plainStone->grow( *voro.getLine( 0 ), ofVec2f( 1920 / 2, 1080 / 2 ) );
	}

	if( doScale ) {
		updateScale();
	}
	//float am = ofMap( ofGetMouseX(), 0, 1920, 0, 2 );
	//float fr = ofMap( ofGetMouseY(), 0, 1080, 0, 2 );
	//noiseWarp->setAmplitude( am );
	//noiseWarp->setFrequency( fr );

	// slowWarpPass->setAmplitude( 0.004 );
	// slowWarpPass->setFrequency( 0.976 );
}

void GrowingBrushStokeAct::updateSecondStone()
{
	secondPlainStone->grow( *voro.getLine( 0 ), ofVec2f( 1920 / 2, 1080 / 2 ) );
}

void GrowingBrushStokeAct::updateScale()
{
	scaleVal += 0.0005f;
	scaleVal = std::min( scaleVal, 2.4f );
}

void GrowingBrushStokeAct::draw() {
	tintBuffer.begin();
	edgeDetectionPostProcessing->begin();

	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	float actualScale = scaleVal + 1.6;
	ofScale( actualScale, actualScale );

	plainStone->setSelectedColor( ofColor( 255 ) );
	plainStone->setTransparency( 255 );
	plainStone->draw( -1920 / 2, -1080 / 2, 1920, 1080 );

	ofPopMatrix();
	
	ofPopStyle();
	edgeDetectionPostProcessing->end();
	tintBuffer.end();

	//whiteLinesBackground.draw( 0, 0 );
	ofPushStyle();
	ofSetColor( 255, transparency );
	tintBuffer.draw( 0, 0 );
	ofPopStyle();
}

void GrowingBrushStokeAct::drawSecondStone()
{
	secondTintBuffer.begin();
	secondEdgeDetectionPass->begin();

	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	float scaleSecondStone = 1.3;
	ofScale( scaleSecondStone, scaleSecondStone );

	secondPlainStone->setSelectedColor( ofColor( 255 ) );
	secondPlainStone->setTransparency( 255 );
	secondPlainStone->draw( -1920 / 2, -1080 / 2, 1920, 1080 );

	ofPopMatrix();

	ofPopStyle();
	secondEdgeDetectionPass->end();
	secondTintBuffer.end();

	ofPushStyle();
	ofSetColor( 255, secondPlainStoneTransparency );
	secondTintBuffer.draw( 0, rockYpos );
	secondTintBuffer.draw( 0, rockYpos - 1080 );

	ofPopStyle();
}

void GrowingBrushStokeAct::keyPressed( int key )
{
	if( key == 'g' ) {
		// 0 means random brush
		//plainStone.growPlain( growBrushIndex );
		plainStone->grow( *voro.getLine( 0 ) );
	}
	else if( key == 'f' ) {
		ofToggleFullscreen();
	}
	else if( key == 'j' ) {
		doScale = !doScale;
	}
	else if( key == 'q' ) {
		growBrushIndex++;
	}
	else if( key == 'w' ) {
		growBrushIndex--;
	}
	else if( key == 'i' ) {
		
	}
}

void GrowingBrushStokeAct::updateFourStones()
{
	
}

void GrowingBrushStokeAct::drawVoronoiWeb()
{
	ofPushStyle();
	ofSetColor( 255, 255 );
	glLineWidth( 3 );
	for( int i = 0; i < dottedPoints.size(); i++ ) {
		//voro2.getLine( i )->draw();
		ActSequencer::drawSplitLines( dottedPoints.at( i ) );
	}
	ofPopStyle();
}

void GrowingBrushStokeAct::lowerScale()
{
	scaleVal -= 0.01f;
	scaleVal = std::max( scaleVal, 0.0f );
	doDrawBackground = true;
}

void GrowingBrushStokeAct::updateRockYpos()
{
	rockYpos += 0.5f;
	if( rockYpos >= 1080 ){
		rockYpos = 0;
	}
}

