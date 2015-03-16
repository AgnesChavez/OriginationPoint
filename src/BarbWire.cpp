#include "BarbWire.h"


BarbWire::BarbWire()
{
	gridResolutionY = 15;
	gridResolutionX = 15;
	distance = 150;
	barbWireKnobSize = 12;
	gridDeformAmount = 10;

	barbWireThickness = 2;

	hueVal = 0;
	transparency = 255;	
}


BarbWire::~BarbWire()
{
}

void BarbWire::draw()
{
	ofPushStyle();
	ofSetColor( ofColor::fromHsb( hueVal, 255, 255 ), transparency );
	glLineWidth( barbWireThickness );
	for( int i = 0; i < xLines.size(); i++ ) {
		
		
		xLines.at( i ).draw();
		yLines.at( i ).draw();
	}

	ofPopStyle();
}

void BarbWire::setTransparency( float _trans )
{
	this->transparency = _trans;
}

float BarbWire::getTransparency()
{
	return transparency;
}

void BarbWire::init()
{
	xLines.clear();
	yLines.clear();

	for( int y = 0; y < gridResolutionY; y++ ) {
		ofPolyline xLine, yLine;
		for( int x = 0; x < gridResolutionX; x++ ) {
			ofPoint firstPointX( distance * x, distance * y );
			ofPoint firstPointY( distance * y, distance * x );
			ofPoint secondPointY( distance * y, distance * ( x + 1 ) );
			ofPoint secondPointX( distance * ( x + 1 ), distance * y );

			ofPoint addToX( ofRandom( -gridDeformAmount, gridDeformAmount ), ofRandom( -gridDeformAmount, gridDeformAmount ) );
			firstPointX += addToX;
			ofPoint addToY( ofRandom( -gridDeformAmount, gridDeformAmount ), ofRandom( -gridDeformAmount, gridDeformAmount ) );
			firstPointY += addToY;
			ofPoint addToX2( ofRandom( -gridDeformAmount, gridDeformAmount ), ofRandom( -gridDeformAmount, gridDeformAmount ) );
			secondPointX += addToX2;
			ofPoint addToY2( ofRandom( -gridDeformAmount, gridDeformAmount ), ofRandom( -gridDeformAmount, gridDeformAmount ) );
			secondPointY += addToY2;

			xLine.curveTo( firstPointX );
			xLine.curveTo( secondPointX );

			yLine.curveTo( firstPointY );
			yLine.curveTo( secondPointY );
			if( x == 0 ) {
				xLine.curveTo( firstPointX );
				xLine.curveTo( secondPointX );

				yLine.curveTo( firstPointY );
				yLine.curveTo( secondPointY );
			}

			// barbwire knot
			for( int i = 0; i < 5; i++ ) {
				float _addx = ofRandom( -barbWireKnobSize, barbWireKnobSize );
				float _addy = ofRandom( -barbWireKnobSize, barbWireKnobSize );
				ofPoint xto = secondPointX + ofPoint( _addx, _addy );
				ofPoint yto = secondPointY + ofPoint( _addx, _addy );
				xLine.curveTo( xto );
				yLine.curveTo( yto );
			}
		}
		xLines.push_back( xLine );
		yLines.push_back( yLine );
	}
}

void BarbWire::setHue( float _hue )
{
	this->hueVal = _hue;
}

float BarbWire::getHue()
{
	return this->hueVal;
}

void BarbWire::setThickness( float _thick )
{
	this->barbWireThickness = _thick;
}

float BarbWire::getThickness()
{
	return this->barbWireThickness;
}
