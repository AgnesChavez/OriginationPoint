#include "ColorCollection.h"


ColorCollection::ColorCollection()
{
}


ColorCollection::~ColorCollection()
{
}

void ColorCollection::addColor( float r, float g, float b )
{
	colors.push_back( ofColor( r, g, b ) );
}

void ColorCollection::addColor( ofColor color )
{
	colors.push_back( color );
}

ofColor ColorCollection::getRandomColor()
{
	return colors.at( ofRandom( 0, colors.size() ) );
}

ofColor ColorCollection::getColorById( int id )
{
	return colors.at( id );
}

int ColorCollection::size()
{
	return colors.size();
}

ColorCollection ColorCollection::getBlackAndWhiteColorCollection()
{
	ColorCollection bwColor;
	bwColor.addColor( ofColor(0, 0, 0) );
	bwColor.addColor( ofColor(255, 255, 255) );
	return bwColor;
}
