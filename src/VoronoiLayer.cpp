#include "VoronoiLayer.h"


VoronoiLayer::VoronoiLayer()
{
	count = 0;
	displayMode = 0;
	transparency = 255;

	buffer.allocate( ofGetWidth(), ofGetHeight() );
}


VoronoiLayer::~VoronoiLayer()
{
}

void VoronoiLayer::addPoint( float x, float y )
{
	pts.push_back( ofVec2f( x, y ) );
}

void VoronoiLayer::compute()
{
	if( pts.size() != count ){
		voronoi.compute( pts, ofRectangle( 0.0f, 0.0f, ofGetWidth(), ofGetHeight() ), 0.0f );
		count = pts.size();

		mesh.clear();
		voronoi.buildMesh( mesh );

		// random face colors
		unsigned int N = mesh.getIndices().size(), M = mesh.getVertices().size();
		vector<ofFloatColor> colors;
		colors.resize( M );
		for( unsigned int i = 0; i < N; i += 3 ){
			ofFloatColor c(
				ofRandom( 1.0f ), // R
				ofRandom( 1.0f ), // G
				ofRandom( 1.0f )  // B
				);
			colors[ mesh.getIndex( i + 0 ) ] = c; // cell center
			colors[ mesh.getIndex( i + 1 ) ] = ofFloatColor( 1.0f, 1.0f, 1.0f, 0.0f ); // cell border
			colors[ mesh.getIndex( i + 2 ) ] = ofFloatColor( 1.0f, 1.0f, 1.0f, 0.0f ); // cell border
		}
		for( unsigned int i = 0; i < M; ++i ) mesh.addColor( colors[ i ] );


	}
}

void VoronoiLayer::draw( float x, float y )
{
	ofPushStyle();
	ofSetColor( 255, transparency );
	buffer.draw( x, y );
	ofPopStyle();
}

void VoronoiLayer::clear()
{
	pts.clear();
	mesh.clear();
	count = 0;
}

void VoronoiLayer::setDisplayMode( int dm )
{
	this->displayMode = dm;
}

void VoronoiLayer::render()
{
	if( count < 1 ) return;

	buffer.begin();
	ofClear( 0.0 );
	ofPushStyle();

	for( unsigned int i = 0; i < pts.size(); ++i ) {
		ofCircle( pts[ i ].x, pts[ i ].y, 4 );
	}

	switch( displayMode ){
	case '1':
		glPointSize( 5.0f );
		mesh.drawVertices();
		// mesh.draw(OF_MESH_POINTS);
		break;
	case '2':
		mesh.drawWireframe();
		// mesh.draw(OF_MESH_WIREFRAME);
		break;
	case '3':
		//mesh.draw(OF_MESH_FILL);
		mesh.drawFaces();
		break;
	default:
		// draw cells
		ofxSegmentIterator it = voronoi.edges();
		for( ; it; ++it ){
			ofxSegment e = *it;
			ofLine( e.p1, e.p2 );
		}
	}

	ofPopStyle();
	buffer.end();
}

std::vector< ofVec2f > VoronoiLayer::getPoints()
{
	return pts;
}

void VoronoiLayer::setTransparency( float _trans )
{
	this->transparency = _trans;
}
