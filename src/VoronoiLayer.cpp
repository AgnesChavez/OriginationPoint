#include "VoronoiLayer.h"


VoronoiLayer::VoronoiLayer()
{
	count = 0;
	transparency = 255;
	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = true;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;

	buffer.allocate( settings );

	int off = 100;

	con = new voro::container( -off, settings.width + (2 * off ),
		-off, settings.height + ( 2 * off ),
		-10, 10,
		1, 1, 1, true, true, true, 3 );

	smoothAmount = 5;
	thickness = 3;

	isDrawn = true;
}


VoronoiLayer::~VoronoiLayer()
{
	delete con;
}

void VoronoiLayer::addPoint( float x, float y )
{
	pts.push_back( ofVec2f( x, y ) );
}

void VoronoiLayer::addRandomPoint()
{
	int x = ofRandom( con->ax, con->bx );
	int y = ofRandom( con->ay, con->by );
	pts.push_back( ofVec2f( x, y ) );
}

void VoronoiLayer::compute()
{
	if( isDrawn ) {
		con->clear();
		for( int i = 0; i < pts.size(); i++ ) {
			addCellSeed( *con, ofPoint( pts.at( i ) ), i, false );
		}

		lines.clear();
		edgePoints.clear();

		edgePoints = getCellsVertices( *con );
		std::vector< ofPoint> centroids = getCellsCentroids( *con );
		for( int k = 0; k < edgePoints.size(); ++k ) {
			std::vector< ofPoint > * selectedPoints = &edgePoints.at( k );
			ofPoint centroid = centroids.at( k );
			std::vector< Point1 > pppps( selectedPoints->size() );
			for( int i = 0; i < selectedPoints->size(); ++i ) {
				Point1 p;
				p.x = selectedPoints->at( i ).x;
				p.y = selectedPoints->at( i ).y;
				pppps.at( i ) = p;
				for( int j = 0; j < selectedPoints->size(); ++j ) {
					ofPoint p1 = selectedPoints->at( i );
					ofPoint p2 = selectedPoints->at( j );
					ofPoint p3 = centroid;
				}
			}

			ofPolyline line;
			line.setClosed( true );
			std::vector< Point1 > ps = convex_hull( pppps );
			for( int i = 0; i < ps.size(); ++i ) {
				Point1 from = ps.at( i );
				Point1 to;
				if( i == ps.size() - 1 ) {
					to = ps.at( 0 );
				}
				else {
					to = ps.at( i + 1 );
				}
				for( float j = 0; j < 1.0; j += 0.1 ) {
					float _x = ofLerp( from.x, to.x, j );
					float _y = ofLerp( from.y, to.y, j );
					line.addVertex( _x, _y );
				}
			}

			line.setClosed( true );
			line = line.getSmoothed( smoothAmount );
			line = line.getResampledBySpacing( 10 );
			lines.push_back( line );
		}

	}
}

void VoronoiLayer::draw( float x, float y )
{
	if( isDrawn ) {
		ofPushStyle();
		ofSetColor( 255, transparency );
		buffer.draw( x, y, 1920, 1080 );
		ofPopStyle();
	}
}

void VoronoiLayer::clear()
{
	pts.clear();
	for( int i = 0; i < lines.size(); i++ ) {
		lines.at( i ).clear();
	}
	count = 0;
}

void VoronoiLayer::render()
{
	buffer.begin();
	ofEnableSmoothing();
	ofClear( 0.0 );
	ofPushStyle();

	for( unsigned int i = 0; i < pts.size(); ++i ) {
		ofCircle( pts[ i ].x, pts[ i ].y, 4 );
	}

	ofSetColor( 255, 0, 0 );
	glLineWidth( thickness );
	for( int i = 0; i < lines.size(); i++ ) {
		lines.at( i ).draw();
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

std::vector< Point1 > VoronoiLayer::convex_hull( std::vector < Point1> points )
{
	int n = points.size(), k = 0;
	vector<Point1> H( 2 * n );

	// Sort points lexicographically
	sort( points.begin(), points.end() );

	// Build lower hull
	for( int i = 0; i < n; ++i ) {
		while( k >= 2 && cross( H[ k - 2 ], H[ k - 1 ], points[ i ] ) <= 0 ) k--;
		H[ k++ ] = points[ i ];
	}

	// Build upper hull
	for( int i = n - 2, t = k + 1; i >= 0; i-- ) {
		while( k >= t && cross( H[ k - 2 ], H[ k - 1 ], points[ i ] ) <= 0 ) k--;
		H[ k++ ] = points[ i ];
	}

	H.resize( k );
	return H;
}

float VoronoiLayer::cross( const Point1 &O, const Point1 &A, const Point1 &B )
{
	return ( A.x - O.x ) * ( B.y - O.y ) - ( A.y - O.y ) * ( B.x - O.x );
}

std::vector< ofPolyline > VoronoiLayer::getLines()
{
	return lines;
}

ofPolyline * VoronoiLayer::getLine( int id )
{
	return &lines.at( id );
}

bool VoronoiLayer::isInside( int id, float _x, float _y )
{
	return lines.at( id ).inside( _x, _y );
}

void VoronoiLayer::setSmoothAmount( int smoothA )
{
	this->smoothAmount = smoothA;
}

void VoronoiLayer::setLineThickness( float _thick )
{
	this->thickness = _thick;
}

float VoronoiLayer::getLineThickness()
{
	return this->thickness;
}

int VoronoiLayer::getSmoothAmount()
{
	return this->smoothAmount;
}

void VoronoiLayer::toggleRender()
{
	isDrawn = !isDrawn;
}

