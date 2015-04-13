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

	movementSpeed = 12;
}


StoneCurtainLayer::~StoneCurtainLayer()
{
}

void StoneCurtainLayer::setup()
{
	stoneCurtainXpos = 1920;
	stoneCurtainXpos2 = -1920;
	stoneCurtainTransparency = 0;
 }

void StoneCurtainLayer::update()
{
	noiseVal += 0.5f;
}

void StoneCurtainLayer::draw()
{
	ofPushStyle();
	ofSetColor( 209, 183, 53, stoneCurtainTransparency );
	/*
	unsigned int currentXPos = stoneCurtainXpos;
	if( stoneCurtainXpos <= -1920 ) {
		int mod = static_cast< int > ( stoneCurtainXpos ) % -1920;
		currentXPos = mod;
	}
	else if( stoneCurtainXpos >= 1920 ) {
		currentXPos = static_cast< int > ( stoneCurtainXpos ) % 1920;
	}
	*/
	stoneCurtainBuffer.draw( stoneCurtainXpos, -70 );
	stoneCurtainBuffer.draw( stoneCurtainXpos + 1920, -70 );

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
