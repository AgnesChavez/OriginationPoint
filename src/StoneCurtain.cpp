#include "StoneCurtain.h"


StoneCurtain::StoneCurtain()
{
	buffer.allocate( ofGetWidth(), ofGetHeight() );
	buffer.begin();
	ofClear( 1.0 );
	buffer.end();
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
	buffer.draw( x, y );
}

void StoneCurtain::render()
{
	ofPolyline pl;
	pl.addVertex( 0, 0 );
	pl.addVertex( 0, ofGetHeight() );
	pl.addVertex( ofGetWidth(), 0 );
	pl.addVertex( ofGetWidth(), ofGetHeight() );
	pl.setClosed( true );

	bool ins = pl.inside( 100, 100 );

	int xStoneCount = 4;
	int yStoneCount = 4;
	for( int y = 0; y < yStoneCount; y++ ) {
		for( int x = 0; x < xStoneCount; x++ ) {
			Stone s;
			s.setBrushCollection( brushes );
			s.setColorCollection( colors );
			float xpos = ofGetWidth() / xStoneCount;
			float ypos = ofGetHeight() / yStoneCount;
			
			s.init( xpos, ypos, pl );
			stones.push_back( s );
		}
	}

	for( int i = 0; i < stones.size(); i++ ) {
		for( int k = 0; k < 100; k++ ) {
			stones.at( i ).grow( pl );
		}
	}
	
	buffer.begin();
	ofEnableAlphaBlending();
	for( int i = 0; i < stones.size(); i++ ) {
		stones.at( i ).draw( 0, 0 );
	}

	buffer.end();


}
