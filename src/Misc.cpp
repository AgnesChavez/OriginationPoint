#include "Misc.h"


Misc::Misc()
{
}


Misc::~Misc()
{
}

std::vector< ofPoint > Misc::getLineSplitPoints( ofPolyline linesToSplit, float stepLength )
{
	std::vector< ofPoint > returnPoints;
	returnPoints.push_back( linesToSplit.getVertices().at( 0 ) );

	ofPolyline line = linesToSplit;
	ofPoint last = line.getVertices().at( 0 );
	for( int j = 0; j < line.getVertices().size(); j++ ) {
		ofPoint p = line.getVertices().at( j );
		float dist = p.distance( last );
		if( dist > stepLength ) {
			ofPoint pos = last;
			ofPoint step = ( p - last ).limit( stepLength );
			while( dist > stepLength ) {
				pos += step;
				returnPoints.push_back( pos );
				dist -= stepLength;
			}
		}

		last = p;
	}
	returnPoints.push_back( line.getVertices().at( line.getVertices().size() - 1 ) );

	return returnPoints;
}

void Misc::drawSplitLines( std::vector< ofPoint > points )
{
	ofPoint prev;
	for( int i = 0; i < points.size(); i++ ) {
		ofPoint p = points.at( i );
		if( i % 2 == 0 ) {
			prev = p;
		}
		else {
			if( prev.x >= 0 && prev.x <= 1920 && prev.y >= 0 && prev.y <= 1080 ) {
				if( p.x >= 0 && p.x <= 1920 && p.y >= 0 && p.y <= 1080 ) {
					ofLine( prev, p );
				}
			}
		}
	}
}

ofFbo::Settings Misc::getDefaultFboSettings()
{
	ofFbo::Settings generalSettings;
	generalSettings.useDepth = true;
	generalSettings.useStencil = false;
	generalSettings.depthStencilAsTexture = true;
	generalSettings.width = 1920;
	generalSettings.height = 1080;
	return generalSettings;
}
