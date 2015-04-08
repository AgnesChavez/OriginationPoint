#include "KinectInteractionManager.h"
#define STRINGIFY(A) #A


KinectInteractionManager::KinectInteractionManager()
{
}


KinectInteractionManager::~KinectInteractionManager()
{
}

void KinectInteractionManager::init()
{
#ifndef KINECT_NOT_CONNCETED

	wrapper.openKinect();
	wrapper.openDepthStream();

	kinectToStoneDistance = 163.3f;
	offset = 1.0f;
	displayKinect = true;

	kinectFbo.allocate( wrapper.depthWidth, wrapper.depthHeight, GL_RGBA );
	kinectFbo2.allocate( wrapper.depthWidth, wrapper.depthHeight, GL_RGBA );
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

#endif // !KINECT_NOT_DETECTED
}

void KinectInteractionManager::update()
{
#ifndef KINECT_NOT_CONNCETED

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
#endif // !KINECT_NOT_DETECTED

}

void KinectInteractionManager::draw()
{
	if( displayKinect ) {
		kinectFbo2.draw( 250, 0 );
	}
}

std::vector< ofxCvBlob > KinectInteractionManager::getBlobs()
{
#ifndef KINECT_NOT_CONNCETED

	if( doBlobDetection ) {
		ofPixels pix;
		kinectFbo2.readToPixels( pix );
		gr.setFromPixels( pix );
		contourFinder.findContours( gr, 0, 1000, 20, false );
		return contourFinder.blobs;
	}

#endif // !KINECT_NOT_DETECTED
	std::vector< ofxCvBlob > blobs;
	return blobs;

}
