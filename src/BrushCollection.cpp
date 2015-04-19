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

		ofFbo fboImage;
		fboImage.allocate( im.width, im.height );
		fboImage.begin();
		ofClear( 1.0f );
		ofEnableAlphaBlending();
		im.draw( 0, 0 );
		fboImage.end();

		fboBrushes.push_back( fboImage );
	}

	circleBrush.loadImage( "brush.png" );
	ownBrush1.loadImage( "own_brush_2.png" );

	brushCount = fboBrushes.size();
}


BrushCollection::~BrushCollection()
{
}

ofFbo BrushCollection::getRandomBrush()
{
	return fboBrushes.at( ofRandom( 0, brushes.size() ) );
}

ofImage BrushCollection::getCircleBrush()
{
	return circleBrush;
}

ofImage BrushCollection::getOwnBrush1()
{
	return ownBrush1;
}

ofFbo BrushCollection::getBrushById( int id )
{
	return fboBrushes.at( id );
}
