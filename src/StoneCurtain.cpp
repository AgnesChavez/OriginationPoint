#include "StoneCurtain.h"


StoneCurtain::StoneCurtain()
{
	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;
	buffer.allocate( settings );
	transparency = 255;
}


StoneCurtain::~StoneCurtain()
{
}

void StoneCurtain::setColorCollection( ColorCollection _c )
{
	this->colors = _c;
}

void StoneCurtain::setBrushCollection( BrushCollection _b )
{
	this->brushes = _b;
}

void StoneCurtain::draw( float x, float y )
{
	ofPushStyle();
	ofSetColor( 255, transparency );
	buffer.draw( x, y );
	ofPopStyle();
}

void StoneCurtain::render()
{
	ofPolyline pl;
	pl.addVertex( 0, 0 );
	pl.addVertex( 0, 1080 );
	pl.addVertex( 1920, 0 );
	pl.addVertex( 1920, 1080 );
	pl.setClosed( true );


	int xStoneCount = 6;
	int yStoneCount = 4;
	for( int y = 0; y < yStoneCount; y++ ) {
		for( int x = 0; x < xStoneCount; x++ ) {
			BrushStone s;
			s.setBrushCollection( brushes );
			s.setColorCollection( colors );
			float xpos = ( 1980 / xStoneCount * x ) + ( 1920 / xStoneCount / 2.0 );
			float ypos = ( 1080 / yStoneCount * y ) + ( 1080 / yStoneCount / 2.0 );
			
			// xpos += ofRandom( -30, 30 );
			// ypos += ofRandom( -30, 30 );

			s.init( xpos, ypos );
			stones.push_back( s );
		}
	}

	for( int i = 0; i < stones.size(); i++ ) {
		for( int k = 0; k < 50; k++ ) {
			stones.at( i ).grow();
		}
	}

	
	buffer.begin();
	ofPushStyle();
	//ofSetColor( 255, 255 );
	ofEnableAlphaBlending();
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).draw( 0, 0, buffer.getWidth(), buffer.getHeight() );
	}
	ofPopStyle();
	buffer.end();

	// not needed anymore, as everything is on the buffer.
	stones.clear();
}

void StoneCurtain::setTransparency( float _trans )
{
	this->transparency = _trans;
}
