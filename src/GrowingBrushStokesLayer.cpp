#include "GrowingBrushStokesLayer.h"


GrowingBrushStokesLayer::GrowingBrushStokesLayer()
{
	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = false;
	settings.depthStencilAsTexture = true;
	settings.width = 1920;
	settings.height = 1080;
	buffer.allocate( settings );
	transparency = 255;

	growRad = 10;

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	agnesColorSelection.addColor( 232, 151, 44 );
	agnesColorSelection.addColor( 84, 18, 0 );
	agnesColorSelection.addColor( 152, 194, 45 );
	agnesColorSelection.addColor( 43, 74, 12 );
	agnesColorSelection.addColor( 46, 31, 1 );
	agnesColorSelection.addColor( 237, 69, 69 );
	agnesColorSelection.addColor( 128, 51, 0 );
	agnesColorSelection.addColor( 110, 18, 0 );
	agnesColorSelection.addColor( 163, 87, 52 );
	agnesColorSelection.addColor( 71, 209, 108 );

	setBrushCollection( brushCollection );
	setColorCollection( agnesColorSelection );

	
	add( 1920 / 4, 1080 / 2 );

}


GrowingBrushStokesLayer::~GrowingBrushStokesLayer()
{
}

void GrowingBrushStokesLayer::growAll()
{
	growRad = growRad + 1.0f;

	canvas.update();

	canvas.beginWaterDraw();

	for( int i = 0; i < stones.size(); i++ ) {
		BrushStone s = stones.at( i );
		s.grow( growRad );
	}

	canvas.endWaterDraw();

	canvas.beginPigmentDraw( canvas.getRandomPigmentId() );

	for( int i = 0; i < stones.size(); i++ ) {
		BrushStone s = stones.at( i );
		s.grow( growRad );
	}

	canvas.endPigmentDraw();
	
	canvas.render();
}

void GrowingBrushStokesLayer::add( float x, float y )
{
	ofPolyline line;
	line.addVertex( 0, 0 );
	line.addVertex( 1920, 0 );
	line.addVertex( 0, 1080 );
	line.addVertex( 1920, 1080 );
	line.setClosed( true );

	BrushStone s;
	s.setColorCollection( colors );
	s.setBrushCollection( brushes );
	s.init( x, y, line );
	stones.push_back( s );
}

void GrowingBrushStokesLayer::setTransparency( float _trans )
{
	this->transparency = _trans;
}

void GrowingBrushStokesLayer::draw( float x, float y )
{
	canvas.draw( x, y );
}

void GrowingBrushStokesLayer::setBrushCollection( BrushCollection _b )
{
	this->brushes = _b;
}

void GrowingBrushStokesLayer::setColorCollection( ColorCollection _c )
{
	this->colors = _c;
	for( int i = 0; i < _c.size(); i++ ) {
		ofColor selected = _c.getColorById( i );
		ofColor inverted = ofColor( 255 - selected.r, 255 - selected.g, 255 - selected.b );
		canvas.addPigment( inverted );
	}
}
