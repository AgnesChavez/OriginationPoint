#include "BrushCollection.h"

BrushCollection::BrushCollection()
{
	for( int i = 1; i < 26; i++ ) {
		std::stringstream ss;
		ss << "water_color_textures_";
		if( i < 10 ) {
			ss << "0";
		}
		ss << i << ".png";
		ofImage im;
		im.loadImage( ss.str() );
		brushes.push_back( im );
	}

	circleBrush.loadImage( "brush.png" );
}


BrushCollection::~BrushCollection()
{
}

ofImage BrushCollection::getRandomBrush()
{
	return brushes.at( ofRandom( 0, brushes.size() ) );
}

ofImage BrushCollection::getCircleBrush()
{
	return circleBrush;
}
