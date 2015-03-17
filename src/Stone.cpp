#include "Stone.h"
#include "omp.h"


Stone::Stone( )
{
	setFuzzy( 10.0f );
	setRadius( 40.0f );
	setSize( 60 );
	transparency = 255;
	borderTransparency = 255;
	brushStokeCount = 50;
	brushStrokeSizeMin = 20;
	brushStrokeSizeMax = 80;
	brushStrokeAlpha = 140;
	saturation = 255;

	borderSize = 30;

	currentGrowRad = 10;
	maxGrowRad = 600;

	bufferWidth = 1920;
	bufferHeight = 1080;
	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = bufferWidth;
	settings.height = bufferHeight;

	tDrawStone = true;
	tDrawBorder = true;

	layer.allocate( settings );
	underlyingLayer.allocate( settings );

	int threadCount = omp_get_max_threads();
	omp_set_num_threads( threadCount );
}

Stone::~Stone()
{
}

void Stone::init( float _x, float _y, ofPolyline line  )
{
	layer.begin();
	ofClear( 0.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 0.0 );
	underlyingLayer.end();

	centroid = ofPoint( _x, _y );
}

void Stone::renderBorder()
{
	underlyingLayer.begin();
	ofPushStyle();
	ofEnableAlphaBlending();
	ofEnableBlendMode( OF_BLENDMODE_ADD );

	if( contourPoints.size() > 0 ) {
		ofFill();
		
		TS_START_NIF( "convert1" );
		std::vector< Point1 > convexPoints( contourPoints.size() );
#pragma omp parallel for 
		for( int i = 0; i < contourPoints.size(); i += 1 ) {
			ofPoint _poi = contourPoints.at( i );
			Point1 p;
			p.x = _poi.x;
			p.y = _poi.y;
			convexPoints.at( i ) = p;
		}
		TS_STOP_NIF( "convert1" );

		TS_START_NIF( "convex" );
		std::vector< Point1 > ps = VoronoiLayer::convex_hull( convexPoints );
		TS_STOP_NIF( "convex" );

		border.clear();
		border.setClosed( true );
	
		TS_START_NIF( "convert2" );
		std::vector< ofPoint > finalPoints( ps.size() );
#pragma omp parallel for 
		for( int i = 0; i < ps.size(); i++ ) {
			Point1 from = ps.at( i );
			finalPoints.at(i ) = ofPoint( from.x, from.y );
		}
		TS_STOP_NIF( "convert2" );

		TS_START_NIF( "etc" );
		border.addVertices( finalPoints );
		
		border.setClosed( true );
		border = border.getResampledBySpacing( 20 );

		ofSetColor( 51, 25, 0, 255 );
		float s = 10;
		ofSeedRandom( 0 );
		TS_STOP_NIF( "etc" );

		TS_START_NIF( "drawing_b" );
		for( int i = 0; i < border.getVertices().size(); i++ ) {
			ofPoint p = border.getVertices().at( i );
			brushes.getRandomBrush().draw( p.x - borderSize / 2.0, p.y - borderSize / 2.0, borderSize, borderSize );
		}
		TS_STOP_NIF( "drawing_b" );

		ofSeedRandom();
	}
	else {
		std::cout << "Contour points empty. No border!" << std::endl;
	}

	ofDisableAlphaBlending();
	ofPopStyle();
	underlyingLayer.end();
}

void Stone::draw( float x, float y )
{
	ofPushStyle();
	ofEnableAlphaBlending();
	
	if( tDrawStone ) {
		ofSetColor( 255, transparency );
		layer.draw( x, y, 1920, 1080 );
	}
	if( tDrawBorder ) {
		ofSetColor( 255, borderTransparency );
		underlyingLayer.draw( x, y, 1920, 1080 );
	}
	ofPopStyle();
}

void Stone::setFuzzy( float fuzzy )
{
	this->fuzzy = fuzzy;
}

void Stone::setRadius( float rad )
{
	this->radius = rad;	
}

float Stone::getRadius()
{
	return this->radius;
}

float Stone::getFuzzy()
{
	return this->fuzzy;
}

void Stone::setSize( int size )
{
	this->size = size;
}

int Stone::getNumberOfCircles()
{
	return this->size;
}


std::vector< ofPoint > Stone::getContourPoints( float x, float y )
{
	return contourPoints;
}

void Stone::calcBorder( std::vector< ofPoint > points )
{
	contourPoints = convexHull.getConvexHull( points );
}

int Stone::getNumberOfStrokes()
{
	return getNumberOfCircles();
}

void Stone::setBrushCollection( BrushCollection _b )
{
	this->brushes = _b;
}

void Stone::clear()
{
	TS_START( "stone_indi_clear" );
	layer.begin();
	ofClear( 1.0 );
	layer.end();

	underlyingLayer.begin();
	ofClear( 1.0 );
	underlyingLayer.end();

	locationsPointsDrawn.clear();
	contourPoints.clear();

	currentGrowRad = 10.0f;
		
	TS_STOP( "stone_indi_clear" );
}

void Stone::grow(ofPolyline line)
{
	if( currentGrowRad < maxGrowRad ) {
		currentGrowRad += 0.5f;

		layer.begin();

		int nrToCheck = ( int ) ( ofMap( currentGrowRad, 0, maxGrowRad, 5, 15 ) );
		ofPushStyle();
		ofEnableAlphaBlending();
		std::vector< ofVec2f > pointsToDraw( nrToCheck );
		ofVec2f p = line.getCentroid2D();
#pragma omp parallel for 
		for( int i = 0; i < nrToCheck; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			//int randomId = ofRandom( 0, points.size() );
			
			ofVec2f pToSave = p + ofVec2f( _x, _y );
			pointsToDraw.at( i ) = pToSave;
		}

		ofPolyline lineToCheck = line.getResampledBySpacing( 50 );
		std::vector< ofPoint > tempLocationsDrawn( nrToCheck);

		// checking parallelized for overlapping
#pragma omp parallel for 
		for( int i = 0; i < pointsToDraw.size(); i++ ) {
			ofVec2f p = pointsToDraw.at( i );
			ofRectangle bb = lineToCheck.getBoundingBox();
			// first check for bounding box inside as its quicker to compute
			if( bb.inside( p ) ) {
				if( lineToCheck.inside( p ) ) {
					tempLocationsDrawn.at( i ) = ofPoint( p.x, p.y );
				}
			}
		}

		// removing vectors at around location 0, 0
		vector<ofPoint>::iterator it = tempLocationsDrawn.begin();
		for( ; it != tempLocationsDrawn.end(); ) {
			ofPoint *_p = &( *it );
			if( _p->x < 4 || _p->y < 4 ) {
				it = tempLocationsDrawn.erase( it );
			}
			else {
				++it;
				locationsPointsDrawn.push_back( *_p );
			}
		}
		
		for( int i = 0; i < tempLocationsDrawn.size(); i++ ) {
			ofVec2f p = tempLocationsDrawn.at( i );
			float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
			ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
			brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );
		}

		ofDisableAlphaBlending();
		ofPopStyle();

		layer.end();

		underlyingLayer.begin();
		ofClear( 1.0 );
		underlyingLayer.end();

		if( tDrawBorder ) {
			TS_START( "indi_border_calc" );
			calcBorder( locationsPointsDrawn );
			TS_STOP( "indi_border_calc" );
			TS_START( "indi_border_render" );
			renderBorder();
			TS_STOP( "indi_border_render" );
		}
	}
}

void Stone::grow()
{
	currentGrowRad += 0.5f;
	if( currentGrowRad < maxGrowRad ) {
		calcBorder( locationsPointsDrawn );

		underlyingLayer.begin();
		ofClear( 1.0 );
		underlyingLayer.end();

		renderBorder();

		layer.begin();

		int nrToCheck = ( int ) ( ofMap( currentGrowRad, 0, maxGrowRad, 5, 15 ) );

		ofPushStyle();
		ofEnableAlphaBlending();
		for( int i = 0; i < nrToCheck; i++ ) {
			float deg = ofRandom( 0, TWO_PI );
			float _x = currentGrowRad * cos( deg );
			float _y = currentGrowRad * sin( deg );
			float s = ofRandom( brushStrokeSizeMin, brushStrokeSizeMax );
			//int randomId = ofRandom( 0, points.size() );
			
			ofVec2f p( centroid );
			//ofVec2f p = getCenterById( randomId );
			p += ofVec2f( _x, _y );

			ofSetColor( colors.getRandomColor(), brushStrokeAlpha );
			locationsPointsDrawn.push_back( ofVec2f( p.x, p.y ) );
			brushes.getRandomBrush().draw( p.x - s / 2.0, p.y - s / 2.0, s, s );

		}
		ofDisableAlphaBlending();
		ofPopStyle();

		layer.end();
	}
}



void Stone::setColorCollection( ColorCollection _c )
{
	this->colors = _c;
}

void Stone::setTransparency( float _trans )
{
	transparency = _trans;
}

void Stone::setBrushStrokeCount( int count )
{
	this->brushStokeCount = count;
}

void Stone::setBrushStrokeSizeMin( float min )
{
	this->brushStrokeSizeMin = min;
}

void Stone::setBrushStrokeSizeMax( float max )
{
	brushStrokeSizeMax = max;
}

void Stone::setBrushStrokeAlpha( float alpha )
{
	brushStrokeAlpha = alpha;
}

int Stone::getBrushStrokeCount()
{
	return this->brushStokeCount;
}

float Stone::getBrushStrokeSizeMin()
{
	return this->brushStrokeSizeMin;
}

float Stone::getBrushStrokeSizeMax()
{
	return this->brushStrokeSizeMax;
}

float Stone::getBrushStrokeAlpha()
{
	return this->brushStrokeAlpha;
}

void Stone::setBorderTransparency( float _trans )
{
	this->borderTransparency = _trans;
}

ofFbo Stone::getStoneBuffer()
{
	return layer;
}

void Stone::setBorderSize( int _bsize )
{
	this->borderSize = _bsize;
}

int Stone::getBorderSize()
{
	return this->borderSize;
}

void Stone::setSaturation( float _sat )
{
	this->saturation = _sat;
}

float Stone::getSaturation()
{
	return this->saturation;
}

vector<ofVec3f> Stone::resamplePolylineToCount( const ofPolyline& polyline, int n )
{
	vector<ofVec3f> points;
	float jump = 1.0f / ( n - 1 );

	for( int i = 0; i < n; i++ ) {
		points.push_back( polyline.getPointAtPercent( min( i*jump, 0.99999f ) ) );
	}
	return points;
}

float Stone::getTransparency()
{
	return this->transparency;
}

void Stone::toggleRenderBorder( bool _b )
{
	this->tDrawBorder = _b;
}

void Stone::toggleDrawStone( bool _s )
{
	this->tDrawStone = _s;
}
