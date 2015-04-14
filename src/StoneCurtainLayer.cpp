#include "StoneCurtainLayer.h"
#include "Misc.h"

StoneCurtainLayer::StoneCurtainLayer()
{
	edge.init( 1920, 1080 );
	edge.createPass<EdgePass>()->setEnabled( true );
	edge.setFlip( false );

	edge2.init( 1920, 1080 );
	edge2.createPass<EdgePass>()->setEnabled( true );
	edge2.setFlip( false );
		
	stoneCurtain.setBrushCollection( brushCollection );
	stoneCurtain.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	stoneCurtain.render();

	stoneCurtain2.setBrushCollection( brushCollection );
	stoneCurtain2.setColorCollection( ColorCollection::getBlackAndWhiteColorCollection() );
	stoneCurtain2.render();

	stoneCurtainBuffer.allocate( Misc::getDefaultFboSettings() );
	stoneCurtainBuffer2.allocate( Misc::getDefaultFboSettings() );

	vectorField.setup( 1920, 1080, 35 );
	vectorField.randomize();

	// drawing stone curtain
	stoneCurtainBuffer.begin();
	ofClear( 0.0, 0.0, 0.0, 1.0 );
	edge.begin();
	stoneCurtain.draw( 0, 0 );
	edge.end();
	stoneCurtainBuffer.end();

	stoneCurtainBuffer2.begin();
	ofClear( 0.0, 0.0, 0.0, 1.0 );
	edge2.begin();
	stoneCurtain2.draw( 0, 0 );
	edge2.end();
	stoneCurtainBuffer2.end();

	setup();

	movementSpeed = 25;

	leftColor = ofColor( 232, 91, 32 );
	rightColor = ofColor( 232, 91, 32 );

	showVectorField = false;
}

StoneCurtainLayer::~StoneCurtainLayer()
{
}

void StoneCurtainLayer::setup()
{
	stoneCurtainXpos = 1920;
	stoneCurtainXpos2 = -1920;
	stoneCurtainTransparency = 0;
	vectorFieldTransparency = 0;
 }

void StoneCurtainLayer::update()
{
	noiseVal += 0.5f;
}

void StoneCurtainLayer::draw()
{
	ofPushStyle();
	ofSetColor( leftColor, stoneCurtainTransparency );
	//ofSetColor( 255, stoneCurtainTransparency );
	stoneCurtainBuffer.draw( stoneCurtainXpos, -70 );
	stoneCurtainBuffer.draw( stoneCurtainXpos + 1920, -70 );

	//ofSetColor( 120, stoneCurtainTransparency );
	ofSetColor( rightColor, stoneCurtainTransparency );
	stoneCurtainBuffer2.draw( stoneCurtainXpos2, 60 );
	stoneCurtainBuffer2.draw( stoneCurtainXpos2 - 1920, 60 );
	ofPopStyle();
}

void StoneCurtainLayer::updateStoneCurtainPos()
{
	float randomSpeed = ofNoise( ofGetElapsedTimef() ) * movementSpeed;
	stoneCurtainXpos -= randomSpeed;
	if( stoneCurtainXpos <= -1920 ) {
		stoneCurtainXpos = 0;
	}

	stoneCurtainXpos2 += randomSpeed;
	if( stoneCurtainXpos2 >= 1920 ) {
		stoneCurtainXpos2 = 0;
	}
}

void StoneCurtainLayer::drawVectorField()
{
	if( showVectorField ) {
		ofPushStyle();
		vectorField.animate( 0.008 );
		vectorField.draw( vectorFieldTransparency );
		ofPopStyle();
	}
}
