#include "NoiseTexture.h"


NoiseTexture::NoiseTexture()
{
	noise.load( "", ofToDataPath( "shader/noiseiq.fs" ) );

	noiseZoom = 3.0f;
	noiseSpeed = 1.0f;

	ofFbo::Settings settings;
	settings.useDepth = true;
	settings.useStencil = true;
	settings.depthStencilAsTexture = false;
	settings.width = 1920;
	settings.height = 1080;

	noiseFbo.allocate( settings );

	w1 = 0.5;
	w2 = 0.25;
	w3 = 0.125;
	w4 = 0.075;
}


NoiseTexture::~NoiseTexture()
{
}
	
void NoiseTexture::render()
{
	float ws[ 8 ] = { 0.2, 0.2, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1 };
	float * weights;
	weights = ws;

	float sc[ 8 ] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.7, 0.8, 0.9 };
	float * scalings;
	scalings = sc;

	noiseFbo.begin();
	ofClear( 0 );
	noise.begin();
	noise.setUniform2f( "resolution", 1920.0, 1080.0 );
	noise.setUniform1f( "time", ofGetElapsedTimef()*noiseSpeed );
	noise.setUniform1f( "w1", w1 );
	noise.setUniform1f( "w2", w2 );
	noise.setUniform1f( "w3", w3 );
	noise.setUniform1f( "w4", w4 );
	noise.setUniform1f( "zPos", noiseZoom );
	noise.setUniform1f( "normalization", 1.0f );
	noise.setUniform1i( "octaves", 4 );
	noise.setUniform1f( "seed", ofMap(ofGetMouseX(), 0, 1920, 0, 1 ) );
	noise.setUniform1fv( "weights", weights, 8 );
	noise.setUniform1fv( "scaling", scalings, 8 );
	glBegin( GL_QUADS );
	glTexCoord2f( 0, 0 ); glVertex3f( 0, 0, 0 );
	glTexCoord2f( noiseFbo.getWidth(), 0 ); glVertex3f( noiseFbo.getWidth(), 0, 0 );
	glTexCoord2f( noiseFbo.getWidth(), noiseFbo.getHeight() ); glVertex3f( noiseFbo.getWidth(), noiseFbo.getHeight(), 0 );
	glTexCoord2f( 0, noiseFbo.getHeight() );  glVertex3f( 0, noiseFbo.getWidth(), 0 );
	glEnd();
	noise.end();
	noiseFbo.end();
}

void NoiseTexture::setSpeed( float _s )
{
	this->noiseSpeed = _s;
}

void NoiseTexture::setZoom( float _z )
{
	this->noiseZoom = _z;
}

void NoiseTexture::draw( int x, int y )
{
	noiseFbo.draw( x, y, 1920, 1080 );
}

ofFbo NoiseTexture::getBuffer()
{
	return noiseFbo;
}
