#include "ManyLayersAct.h"

ManyLayersAct::ManyLayersAct()
{
	setup();
}


ManyLayersAct::~ManyLayersAct()
{
}

void ManyLayersAct::setup()
{

	background.loadImage( "watercolor_rocks_pattern.jpg" );
	gridImage.loadImage( "grid_invert.jpg" );

	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	waterPostProcessing.init( 1920, 1080 );
	waterPass = waterPostProcessing.createPass<NoiseWarpPass>();
	waterPass->setEnabled( true );
	waterPostProcessing.setFlip( false );

	waterPass->setAmplitude( 0.009 );
	waterPass->setFrequency( 0.976 );

	transparency = 255;

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	agnesColorSelection.addColor( 232, 151, 44 );
	agnesColorSelection.addColor( 84, 18, 0 );
	agnesColorSelection.addColor( 152, 194, 45 );
	agnesColorSelection.addColor( 43, 74, 12 );
	agnesColorSelection.addColor( 46, 31, 1 );
	agnesColorSelection.addColor( 237, 69, 69 );
	agnesColorSelection.addColor( 128, 51, 0 );
	agnesColorSelection.addColor( 110, 18, 0 );
	agnesColorSelection.addColor( 163, 87, 52 );
	agnesColorSelection.addColor( 71, 209, 108 );


	// adds four stones
	voroFor4Stones.clear();
	ofVec2f rand1( 650 + ofRandom( -100, 100 ), 250 );
	ofVec2f rand2( 1250 + ofRandom( -100, 100 ), 250 );
	ofVec2f rand3( 650 + ofRandom( -100, 100 ), 750 );
	ofVec2f rand4( 1250 + ofRandom( -100, 100 ), 750 );
	voroFor4Stones.addPoint( rand1.x, rand1.y );
	voroFor4Stones.addPoint( rand2.x, rand2.y );
	voroFor4Stones.addPoint( rand3.x, rand3.y );
	voroFor4Stones.addPoint( rand4.x, rand4.y );

	for( int i = 0; i < 5; i++ ) {
		voroFor4Stones.addPoint( ofRandom( 0, 1920 ), 0 );
		voroFor4Stones.addPoint( ofRandom( 0, 1920 ), 1080 );
		voroFor4Stones.addPoint( 0, ofRandom( 0, 1080 ) );
		voroFor4Stones.addPoint( 1920, ofRandom( 0, 1080 ) );
	}

	voroFor4Stones.addPoint( 1920 / 2, 1080 / 2 );

	voroFor4Stones.setSmoothAmount( 25 );
	voroFor4Stones.compute();
	//voroFor4Stones.render();


	// creating the four brush stones
	BrushStone s;
	s.setColorCollection( blackWhiteColor );
	s.setBrushCollection( brushCollection );
	s.setBrushStrokeAlpha( 255 );
	s.init( rand1.x, rand1.y );

	BrushStone s2;
	s2.setColorCollection( blackWhiteColor );
	s2.setBrushCollection( brushCollection );
	s2.setBrushStrokeAlpha( 255 );
	s2.init( rand2.x, rand2.y );

	BrushStone s3;
	s3.setColorCollection( blackWhiteColor );
	s3.setBrushCollection( brushCollection );
	s3.setBrushStrokeAlpha( 255 );
	s3.init( rand3.x, rand3.y );

	BrushStone s4;
	s4.setColorCollection( blackWhiteColor );
	s4.setBrushCollection( brushCollection );
	s4.setBrushStrokeAlpha( 255 );
	s4.init( rand4.x, rand4.y );

	fourStonesLayer.push_back( s );
	fourStonesLayer.push_back( s2 );
	fourStonesLayer.push_back( s3 );
	fourStonesLayer.push_back( s4 );

	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( agnesColorSelection );
	stoneCurtain.render();

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	stoneCurtainBuffer.allocate( settings );

	curtainX = -1920;
	stoneCurtainTransparency = 0;
}

void ManyLayersAct::update()
{
	for( int i = 0; i < fourStonesLayer.size(); i++ ) {
		fourStonesLayer.at( i ).grow( *voroFor4Stones.getLine( i ) );
	}

	curtainX += 3;
	if( curtainX >= 0 ) {
		curtainX = -1920;
	}
}

void ManyLayersAct::draw()
{
	edgeDetectionPostProcessing.begin();
	for( int i = 0; i < fourStonesLayer.size(); i++ ) {
		fourStonesLayer.at( i ).draw( 0, 0, 1920, 1080 );
	}
	edgeDetectionPostProcessing.end( false );

	ofSetColor( 255, transparency );
	
	waterPostProcessing.begin();
	background.draw( 0, 0, 1920, 1080 );

	waterPostProcessing.end();

	edgeDetectionPostProcessing.draw();

	// drawing stone curtain
	stoneCurtainBuffer.begin();
	edgeDetectionPostProcessing.begin();
	stoneCurtain.draw( 0, 0 );
	edgeDetectionPostProcessing.end();
	stoneCurtainBuffer.end();

	ofPushStyle();
	ofSetColor( 152, 194, 45, stoneCurtainTransparency );
	stoneCurtainBuffer.draw( curtainX + 1920, 0 );
	stoneCurtainBuffer.draw( curtainX, 0 );
	ofPopStyle();
}
