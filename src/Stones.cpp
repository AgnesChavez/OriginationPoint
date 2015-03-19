#include "Stones.h"


Stones::Stones()
{
}


Stones::~Stones()
{
}

void Stones::init()
{
	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	buffer.allocate( settings );
}

void Stones::render( std::vector< ofPolyline > closedLines )
{
	buffer.begin();
	ofBackground( 0 );
	ofSetColor( 255, 255 );
	ofFill();
	for( int i = 0; i < closedLines.size(); i++ ) {
		ofPolyline line = closedLines.at( i );
		line.setClosed( true );
		
		ofBeginShape();
		std::vector<ofPoint>& vertices = line.getVertices();
		for( int j = 0; j < vertices.size(); j++ ) {
			ofVertex( vertices[ j ] );
		}
		ofEndShape();
	}

	buffer.end();

	
	/*
	ofPixels pix;
	buffer.readToPixels( pix );
	ofImage im;
	im.setFromPixels( pix );
	im.saveImage( "outtest.png" );
	ofExit( 1 );
	*/
}

ofFbo Stones::getBuffer()
{
	return buffer;
}
