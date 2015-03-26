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

	stone8ColorCollection.addColor( 236, 73, 78 );
	stone8ColorCollection.addColor( 197, 153, 72 );
	stone8ColorCollection.addColor( 211, 182, 60 );
	stone8ColorCollection.addColor( 197, 190, 51 );
	stone8ColorCollection.addColor( 202, 120, 78 );
	stone8ColorCollection.addColor( 152, 103, 100 );

	brightYellowColorCollection.addColor( 195, 189, 75 );
	brightYellowColorCollection.addColor( 150, 144, 70 );
	brightYellowColorCollection.addColor( 73, 78, 46 );
	brightYellowColorCollection.addColor( 201, 198, 85 );
	brightYellowColorCollection.addColor( 209, 202, 95 );

	brownColorCollection.addColor( 102, 51, 0 );
	brownColorCollection.addColor( 153, 102, 51 );
	brownColorCollection.addColor( 187, 153, 102 );

	blackWhiteColor.addColor( 90, 90, 90 );
	blackWhiteColor.addColor( 255, 255, 255 );

	setBrushCollection( brushCollection );
	setColorCollection( blackWhiteColor );

	
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

	canvas.beginPigmentDraw( 0 );

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
		canvas.addPigment( _c.getColorById( i ) );
	}
}
