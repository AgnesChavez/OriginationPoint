#include "GrowingBrushStokeAct.h"
#include "ActSequencer.h"

GrowingBrushStokeAct::GrowingBrushStokeAct() :
transparency( 255 )
{
	for( int i = 0; i < 80; i++ ) {
		std::vector< ofPoint > emptyPoints;
		dottedPoints.push_back( emptyPoints );
	}

	setup();

}

GrowingBrushStokeAct::~GrowingBrushStokeAct()
{

}


void GrowingBrushStokeAct::setup() {
	voronoiWebTransparency = 0;
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

	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	background.loadImage( "lines_3_bw.jpg" );

	doScale = false;
	scaleNoiseVal = 0.0f;
	rotateNoiseVal = 0.0f;
	growBrushIndex = 0;
	scaleVal = 0.0;
	
	ofBackground( 0 );

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	tintBuffer.allocate( settings );
	voroWebLayer.allocate( settings );
	voroWebLayer.begin();
	drawVoronoiWeb();
	voroWebLayer.end();

	addCustomVoronoiPoints();
}

void GrowingBrushStokeAct::createStone( ofPoint centerStone )
{
	plainStone.setColorCollection( blackWhiteColor );
	plainStone.setBrushCollection( brushCollection );
	plainStone.setBrushStrokeAlpha( 255 );
	plainStone.init( centerStone.x, centerStone.y );
	plainStone.setBrushStrokeSizeMin( 10 );
	plainStone.setBrushStrokeSizeMax( 40 );
	for( int i = 0; i < 3; i++ ) {
		plainStone.grow( *voro.getLine( 0 ), ofVec2f( 1920 / 2, 1080 / 2 ) );
	}
	plainStone.setBrushStrokeSizeMin( 20 );
	plainStone.setBrushStrokeSizeMax( 80 );
}

void GrowingBrushStokeAct::addCustomVoronoiPoints() {

	voro.clear();

	voro.addPoint( 1920 / 2, 1080 / 2 );
	voro.addPoint( ofRandom( 100, 300 ), 100 );
	voro.addPoint( ofRandom( 300, 700 ), 100 );
	voro.addPoint( ofRandom( 700, 1200 ), 100 );
	voro.addPoint( ofRandom( 1200, 1600 ), 100 );
	voro.addPoint( ofRandom( 1600, 1920 ), 100 );

	voro.addPoint( ofRandom( 100, 500 ), 1000 );
	voro.addPoint( ofRandom( 500, 1000 ), 1000 );
	voro.addPoint( ofRandom( 1000, 1900 ), 1000 );

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
		plainStone.grow( *voro.getLine( 0 ), ofVec2f( 1920/2, 1080/2 ) );
	}

	if( doScale ) {
		updateScale();
	}
	// slowWarpPass->setAmplitude( 0.004 );
	// slowWarpPass->setFrequency( 0.976 );
}

void GrowingBrushStokeAct::updateScale()
{
	scaleVal += 0.0005f;
	scaleVal = std::min( scaleVal, 2.4f );
}

void GrowingBrushStokeAct::draw() {

	tintBuffer.begin();
	ofClear( 0, 0, 0, 0 );
	edgeDetectionPostProcessing.begin();
	ofPushStyle();
	ofPushMatrix();
	ofTranslate( 1920 / 2, 1080 / 2, 0 );
	float actualScale = scaleVal + 1.6;
	ofScale( actualScale, actualScale );

	plainStone.setSelectedColor( ofColor( 255 ) );
	plainStone.setTransparency( 200 );
	plainStone.draw( -1920 / 2, -1080/2, 1920, 1080);

	ofPopMatrix();
	
	ofPopStyle();
	edgeDetectionPostProcessing.end();
	tintBuffer.end();

	//edgeDetectionPostProcessing.begin();
	ofPushStyle();
	//ofSetColor( 255, 255 );
	ofSetColor( 255, transparency );
	tintBuffer.draw( 0, 0 );
	ofPopStyle();
	//edgeDetectionPostProcessing.end();

	//ofPushStyle();
	//ofSetColor( 255, transparency );
	//slowWarp.draw();
	//ofPopStyle();

	//fourRocks.draw();
	//eightRocks.draw();
	TS_START( "voro_web" );
	ofPushStyle();
	ofSetColor( 255, voronoiWebTransparency );
	voroWebLayer.draw( 0, 0 );
	ofPopStyle();
	TS_STOP( "voro_web" );
}

void GrowingBrushStokeAct::keyPressed( int key )
{
	if( key == 'g' ) {
		// 0 means random brush
		//plainStone.growPlain( growBrushIndex );
		plainStone.grow( *voro.getLine( 0 ) );
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

void GrowingBrushStokeAct::updateVoronoiWeb( int speed )
{
	voronoiWebTransparency += speed;
}

void GrowingBrushStokeAct::drawVoronoiWeb()
{
	ofPushStyle();
	ofSetColor( 255, 255 );
	for( int i = 0; i < dottedPoints.size(); i++ ) {
		//voro2.getLine( i )->draw();
		ActSequencer::drawSplitLines( dottedPoints.at( i ) );
	}
	ofPopStyle();
}

