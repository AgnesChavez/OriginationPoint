#include "KinectInteractionManager.h"
#include "Misc.h"
#define STRINGIFY(A) #A

#include <ofxCv/libs/ofxCv/include/ofxCv/Wrappers.h>


KinectInteractionManager::KinectInteractionManager()
{
}


KinectInteractionManager::~KinectInteractionManager()
{
}

void KinectInteractionManager::init()
{

	minBlobSize = 100;
	maxBlobSize = 5000;

	wrapper.openKinect();
	wrapper.openDepthStream();

	kinectToStoneDistance = 163.3f;
	offset = 1.0f;
	displayKinect = true;

	ofFbo::Settings generalSettings;
	generalSettings.useDepth = true;
	generalSettings.useStencil = false;
	generalSettings.depthStencilAsTexture = true;
	generalSettings.width = wrapper.depthWidth;
	generalSettings.height = wrapper.depthHeight;


	kinectFbo.allocate( generalSettings );
	kinectFbo2.allocate( generalSettings );

	gr.allocate( generalSettings.width, generalSettings.height );
	std::string fragShaderSrc = STRINGIFY(
		// fragment shader
		uniform float min;
		uniform float max;

		// this is how we receive the texture
		uniform sampler2DRect tex;

		void main()
		{
			vec4 colToSet = vec4( 0, 0, 0, 1 );

			vec2 st = gl_TexCoord[ 0 ].st;
			vec3 col = texture2DRect( tex, st ).rgb;
			if( col.x > min ) {
				if( col.x < max ) {
					colToSet = vec4( 1, 1, 1, 1 );
				}
			}
			gl_FragColor = colToSet;
		}
		);

		std::string vertShaderSrc = STRINGIFY(
			void main() {
			gl_TexCoord[ 0 ] = gl_MultiTexCoord0;
			gl_Position = ftransform();
		}
	);

	kinectShader.setupShaderFromSource( GL_VERTEX_SHADER, vertShaderSrc );
	kinectShader.setupShaderFromSource( GL_FRAGMENT_SHADER, fragShaderSrc );
	kinectShader.linkProgram();
}

void KinectInteractionManager::update()
{
	bool isUpdated = wrapper.updateDepthFrame();

	if( isUpdated ) {
		wrapper.grayscaleImage.erode();
		wrapper.grayscaleImage.dilate();

		kinectFbo.begin();
		ofClear( 0, 0, 0, 255 );
		wrapper.grayscaleImage.draw( 0, 0 );
		kinectFbo.end();
		kinectFbo2.begin();
		ofClear( 0, 255 );
		kinectShader.begin();
		kinectShader.setUniform1f( "min", ( float ) ( ( kinectToStoneDistance - offset ) / 255.0 ) );
		kinectShader.setUniform1f( "max", ( float ) ( ( kinectToStoneDistance + offset ) / 255.0 ) );
		kinectShader.setUniformTexture( "tex", kinectFbo.getTextureReference(), 0 );
		glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 ); glVertex3f( 0, 0, 0 );
		glTexCoord2f( wrapper.grayscaleImage.width, 0 ); glVertex3f( wrapper.grayscaleImage.width, 0, 0 );
		glTexCoord2f( wrapper.grayscaleImage.width, wrapper.grayscaleImage.height ); glVertex3f( wrapper.grayscaleImage.width, wrapper.grayscaleImage.height, 0 );
		glTexCoord2f( 0, wrapper.grayscaleImage.height );  glVertex3f( 0, wrapper.grayscaleImage.height, 0 );
		glEnd();
		kinectShader.end();
		kinectFbo2.end();
	}

	kinectFbo2.readToPixels( pix );
	ofxCv::convertColor( pix, gr.getPixelsRef(), CV_RGBA2GRAY );
	gr.threshold( 120 );
	contourFinder.findContours( gr, minBlobSize, maxBlobSize, 4, false, true );
}

void KinectInteractionManager::draw()
{
	if( displayKinect ) {
		kinectFbo2.draw( 0, 0 );
		ofPushStyle();
		ofNoFill();
		//ofSetColor( 255, 0, 0 );
		for( int i = 0; i < contourFinder.blobs.size(); i++ )
		{
			ofPolyline cur1;
			// add all the current vertices to cur polyline
			cur1.addVertices( contourFinder.blobs[ i ].pts );
			cur1.setClosed( true );
			//cur.draw();
			ofSetColor( 0, 0, 255 );
			drawWithNormals( cur1 );
			ofPolyline smoothed = cur1.getSmoothed( 8 );
			ofSetColor( 0, 255, 255 );
			drawWithNormals( smoothed );

			ofSetColor( 255, 0, 0 );
			ofRect( smoothed.getBoundingBox() );
		}
		ofPopStyle();
	}
}

std::vector< ofxCvBlob > KinectInteractionManager::getBlobs()
{
	return contourFinder.blobs;
}

void KinectInteractionManager::drawWithNormals( const ofPolyline & polyline )
{
	for( int i = 0; i < ( int ) polyline.size(); i++ ) {
		bool repeatNext = i == ( int ) polyline.size() - 1;

		const ofPoint& cur = polyline[ i ];
		const ofPoint& next = repeatNext ? polyline[ 0 ] : polyline[ i + 1 ];

		float angle = atan2f( next.y - cur.y, next.x - cur.x ) * RAD_TO_DEG;
		float distance = cur.distance( next );

		if( repeatNext ) {
			ofSetColor( 255, 0, 255 );
		}
		glPushMatrix();
		glTranslatef( cur.x, cur.y, 0 );
		ofRotate( angle );
		ofLine( 0, 0, 0, distance );
		ofLine( 0, 0, distance, 0 );
		glPopMatrix();
	}
}
