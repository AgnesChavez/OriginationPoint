#include "ManyLayersAct.h"

ManyLayersAct::ManyLayersAct()
{
	background.loadImage( "watercolor_rocks_pattern.jpg" );
	gridImage.loadImage( "grid_invert.jpg" );

	edgeDetectionPostProcessing.init( 1920, 1080 );

	edgePass = edgeDetectionPostProcessing.createPass< EdgePass >();
	edgePass->setEnabled( true );
	edgeDetectionPostProcessing.setFlip( false );

	stoneCurtainEdgeDetectionBuffer.init( 1920, 1080 );
	stoneCurtainEdgeDetectionBuffer.createPass<EdgePass>()->setEnabled( true );
	stoneCurtainEdgeDetectionBuffer.setFlip( false );


	waterPostProcessing.init( 1920, 1080 );
	waterPass = waterPostProcessing.createPass<NoiseWarpPass>();
	waterPass->setEnabled( true );
	waterPostProcessing.setFlip( false );

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

	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( agnesColorSelection );
	stoneCurtain.render();

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = true;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	stoneCurtainBuffer.allocate( settings );
	fourStonesBuffer.allocate( settings );

	// drawing stone curtain
	stoneCurtainBuffer.begin();
	ofClear( 0.0, 0.0, 0.0, 1.0 );
	stoneCurtainEdgeDetectionBuffer.begin();
	stoneCurtain.draw( 0, 0 );
	stoneCurtainEdgeDetectionBuffer.end();
	stoneCurtainBuffer.end();

	setup();
}


ManyLayersAct::~ManyLayersAct()
{
}

void ManyLayersAct::setup()
{
	waterPass->setAmplitude( 0.009 );
	waterPass->setFrequency( 0.976 );

	stoneCurtainXpos = 1915;
	stoneCurtainTransparency = 0;
	fourStonesPos = 0;
	transparency = 0;
	doLoop4Stones = false;
 }

void ManyLayersAct::update()
{
	
}

void ManyLayersAct::draw()
{
	// draw background water effect
	/*
	ofPushStyle();
	ofSetColor( 255, transparency );
	waterPostProcessing.begin();
	background.draw( 0, 0, 1920, 1080 );
	waterPostProcessing.end();
	ofPopStyle();
	*/
	
	// draw stone curtain
	ofPushStyle();
	ofSetColor( 255, stoneCurtainTransparency );
	stoneCurtainBuffer.draw( stoneCurtainXpos, 0 );
	stoneCurtainBuffer.draw( stoneCurtainXpos + 1920, 0 );
	ofPopStyle();

}

void ManyLayersAct::updateStoneCurtainPos()
{
	stoneCurtainXpos -= 0.5f;
	if( stoneCurtainXpos <= -1920 ) {
		stoneCurtainXpos = 0;
	}
}

void ManyLayersAct::updateFourStonesPos()
{
	fourStonesPos += 2;
	if( fourStonesPos > 1920 ) {
		fourStonesPos = 0;
	}
}
