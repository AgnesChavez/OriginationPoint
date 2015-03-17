#pragma once

#include "ofMain.h"
#include <kinect.h>
#include <Kinect.Face.h>
#include <Windows.h>
#include "ofxOpenCv.h"
#include <iostream>
#include <vector>

template<class Interface>
inline void SafeRelease( Interface *& pInterfaceToRelease )
{
	if( pInterfaceToRelease != NULL ){
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

static const DWORD c_FaceFrameFeatures =
FaceFrameFeatures::FaceFrameFeatures_BoundingBoxInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_PointsInColorSpace
| FaceFrameFeatures::FaceFrameFeatures_RotationOrientation
| FaceFrameFeatures::FaceFrameFeatures_Happy
| FaceFrameFeatures::FaceFrameFeatures_RightEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_LeftEyeClosed
| FaceFrameFeatures::FaceFrameFeatures_MouthOpen
| FaceFrameFeatures::FaceFrameFeatures_MouthMoved
| FaceFrameFeatures::FaceFrameFeatures_LookingAway
| FaceFrameFeatures::FaceFrameFeatures_Glasses
| FaceFrameFeatures::FaceFrameFeatures_FaceEngagement;


class KinectWrapper
{
public:
	KinectWrapper();
	~KinectWrapper();

	bool initEverything();

	bool openKinect();

	bool openColorStream();
	bool openDepthStream();
	bool openBodyStream();
	bool openFaceStream();
	bool openHdFaceStream();

	void updateColorFrame();

	bool updateDepthFrame();

	void updateSkeleton();
	Joint * getJointData( unsigned int bodyIndex, JointType jointType );

	void updateFace();
	IFaceFrame * getFaceFrame( unsigned int faceId );
	IFaceFrameResult * getFaceFrameResult( unsigned int faceId );
	void releaseFaceFrame( unsigned int faceId );
	void releaseFaceFrameResult( unsigned int faceId );

	void updateHdFace();
	IHighDefinitionFaceFrame * getHdFaceFrame( unsigned int faceId );
	void releaseHdFaceFrame( unsigned int faceId );


	IKinectSensor * pSensor;

	// color
	IColorFrameSource * pColorSource;
	IColorFrameReader * pColorReader;
	IFrameDescription * pColorDescription;
	//IColorFrame * pColorFrame;
	int colorWidth, colorHeight, colorBufferSize;
	cv::Mat * colorBufferMat, *colorMat;
	ofxCvColorImage colorscaleImage;

	// depth
	IDepthFrameSource* pDepthSource;
	IDepthFrameReader* pDepthReader;
	IFrameDescription* pDepthDescription;
	int depthWidth, depthHeight;
	unsigned int depthBufferSize;
	cv::Mat * bufferMat, *depthMat;
	ofxCvGrayscaleImage grayscaleImage;

	// body
	IBodyFrameSource* pBodySource;
	IBodyFrameReader* pBodyReader;
	IBodyFrame * pBodyFrame;
	IBody* pBodies[ BODY_COUNT ];
	Joint * pJoints[ BODY_COUNT ][ JointType::JointType_Count ];
	HRESULT UpdateBodyData( IBody** ppBodies );
	bool pHaveBodyData;

	// face
	IFaceFrameSource* pFaceSource[ BODY_COUNT ];
	IFaceFrameReader *pFaceReader[ BODY_COUNT ];
	IFaceFrame * pFaceFrame[ BODY_COUNT ];
	IFaceFrameResult * pFaceFrameResult[ BODY_COUNT ];

	// hd face
	IHighDefinitionFaceFrameSource *pHdFaceSource[ BODY_COUNT ];
	IHighDefinitionFaceFrameReader *pHdFaceReader[ BODY_COUNT ];
	IHighDefinitionFaceFrame * pHdFaceFrame[ BODY_COUNT ];
	IFaceModelBuilder * pHdFaceModelBuilder[ BODY_COUNT ];
	IFaceAlignment* pFaceAlignment[ BODY_COUNT ];
	IFaceModel* pFaceModel[ BODY_COUNT ];
	ICoordinateMapper* pCoordinateMapper;
	UINT32 pFaceVertexCount;
	std::vector< std::vector< float > > deformations;
	std::vector< CameraSpacePoint > pHdFacePoints[ BODY_COUNT ];
	bool produce[ BODY_COUNT ];
};

