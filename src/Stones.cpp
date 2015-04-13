#include "Stones.h"
#include "Misc.h"

Stones::Stones()
{
}


Stones::~Stones()
{
}

void Stones::init()
{
	buffer.allocate( Misc::getDefaultFboSettings() );
}

void Stones::render( std::vector< ofPolyline > closedLines, std::vector< float > transparencies, ofPoint centered )
{
	buffer.begin();
	ofClear( 1.0 );
	ofFill();
	for( int i = 0; i < closedLines.size(); i++ ) {
		ofPolyline line = closedLines.at( i );
		line.setClosed( true );
		ofSetColor( 255, transparencies.at( i ) );
		float distance = ofDist( line.getCentroid2D().x, line.getCentroid2D().y, centered.x, centered.y );
		//std::cout << distance << std::endl;
		if( distance < 20 ) {
			ofSetColor( 255, 255 );
		}
		ofBeginShape();
		std::vector< ofPoint >& vertices = line.getVertices();
		for( int j = 0; j < vertices.size(); j++ ) {
			ofVertex( vertices[ j ] );
		}
		ofEndShape();
	}

	buffer.end();
}

ofFbo Stones::getBuffer()
{
	return buffer;
}
