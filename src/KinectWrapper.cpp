#include "KinectWrapper.h"


KinectWrapper::KinectWrapper() :
deformations( BODY_COUNT, std::vector<float>( FaceShapeDeformations::FaceShapeDeformations_Count ) )
{
}


KinectWrapper::~KinectWrapper()
{
}

bool KinectWrapper::initEverything()
{
	HRESULT hResult = S_OK;

	hResult = openKinect();
	hResult = openColorStream();
	hResult = openDepthStream();
	hResult = openBodyStream();
	hResult = openFaceStream();
	//hResult = openHdFaceStream();

	return SUCCEEDED( hResult );
}

bool KinectWrapper::openKinect()
{
	HRESULT hResult = S_OK;

	hResult = GetDefaultKinectSensor( &this->pSensor );
	if( FAILED( hResult ) ){
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
	}

	hResult = this->pSensor->Open();
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
	}

	return SUCCEEDED( hResult );
}

bool KinectWrapper::openColorStream()
{
	HRESULT hResult = S_OK;

	hResult = this->pSensor->get_ColorFrameSource( &this->pColorSource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
	}

	hResult = this->pColorSource->OpenReader( &this->pColorReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
	}
	else {
		std::cout << "Success: IColorFrameSource::OpenReader" << std::endl;
	}

	hResult = pColorSource->get_FrameDescription( &this->pColorDescription );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
	}

	this->pColorDescription->get_Width( &colorWidth );
	this->pColorDescription->get_Height( &colorHeight );
	this->colorBufferSize = colorWidth * colorHeight * 4 * sizeof( unsigned char );

	this->colorMat = new cv::Mat( colorHeight, colorWidth, CV_8UC4 );
	this->colorBufferMat = new cv::Mat( colorHeight, colorWidth, CV_8UC4 );
	this->colorscaleImage.allocate( colorWidth, colorHeight );

	return SUCCEEDED( hResult );
}

bool KinectWrapper::openDepthStream()
{
	HRESULT hResult = S_OK;

	hResult = pSensor->get_DepthFrameSource( &this->pDepthSource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
	}

	hResult = this->pDepthSource->OpenReader( &this->pDepthReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
	}

	hResult = pDepthSource->get_FrameDescription( &this->pDepthDescription );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
	}

	this->pDepthDescription->get_Width( &depthWidth ); // 512
	this->pDepthDescription->get_Height( &depthHeight ); // 424
	this->depthBufferSize = depthWidth * depthHeight * sizeof( unsigned short );

	this->grayscaleImage.allocate( depthWidth, depthHeight );
	this->bufferMat = new cv::Mat( depthHeight, depthWidth, CV_16SC1 );
	this->depthMat = new cv::Mat( depthHeight, depthWidth, CV_8UC1 );

	return SUCCEEDED( hResult );
}

bool KinectWrapper::openBodyStream()
{
	HRESULT hResult = S_OK;

	hResult = pSensor->get_BodyFrameSource( &pBodySource );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IBodyFrameSource::get_BodyFrameSource()" << std::endl;
	}

	hResult = pBodySource->OpenReader( &pBodyReader );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IBodyFrameReader::OpenReader()" << std::endl;
	}

	return SUCCEEDED( hResult );
}

bool KinectWrapper::openFaceStream()
{
	for( int i = 0; i < BODY_COUNT; i++ )
	{
		HRESULT hResult = S_OK;
		hResult = CreateFaceFrameSource( pSensor, 0, c_FaceFrameFeatures, &pFaceSource[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : CreateFaceFrameSource" << std::endl;
			return false;
		}

		hResult = pFaceSource[ i ]->OpenReader( &pFaceReader[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IFaceFrameSource::OpenReader" << std::endl;
			return false;
		}
	}

	return true;
}

bool KinectWrapper::openHdFaceStream()
{
	HRESULT hResult = S_OK;

	hResult = pSensor->get_CoordinateMapper( &pCoordinateMapper );
	if( FAILED( hResult ) ){
		std::cerr << "Error : IKinectSensor::get_CoordinateMapper()" << std::endl;
		return false;
	}

	hResult = GetFaceModelVertexCount( &pFaceVertexCount ); // 1347
	if( FAILED( hResult ) ){
		std::cerr << "Error : GetFaceModelVertexCount()" << std::endl;
		return false;
	}

	for( int i = 0; i < BODY_COUNT; i++ ) {
		HRESULT hResult = S_OK;

		hResult = CreateHighDefinitionFaceFrameSource( pSensor, &pHdFaceSource[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : CreateHighDefinitionFaceFrameSource()" << std::endl;
			return false;
		}

		// Reader
		hResult = pHdFaceSource[ i ]->OpenReader( &pHdFaceReader[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenReader()" << std::endl;
			return false;
		}

		// Open Face Model Builder
		hResult = pHdFaceSource[ i ]->OpenModelBuilder( FaceModelBuilderAttributes::FaceModelBuilderAttributes_None, &pHdFaceModelBuilder[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : IHighDefinitionFaceFrameSource::OpenModelBuilder()" << std::endl;
			return false;
		}

		// Start Collection Face Data
		hResult = pHdFaceModelBuilder[ i ]->BeginFaceDataCollection();
		if( FAILED( hResult ) ){
			std::cerr << "Error : IFaceModelBuilder::BeginFaceDataCollection()" << std::endl;
			return false;
		}

		// Create Face Alignment
		hResult = CreateFaceAlignment( &pFaceAlignment[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : CreateFaceAlignment()" << std::endl;
			return false;
		}

		// Create Face Model
		hResult = CreateFaceModel( 1.0f, FaceShapeDeformations::FaceShapeDeformations_Count, &deformations[ i ][ 0 ], &pFaceModel[ i ] );
		if( FAILED( hResult ) ){
			std::cerr << "Error : CreateFaceModel()" << std::endl;
			return false;
		}

		pHdFacePoints[ i ].reserve( pFaceVertexCount );
	}

	return SUCCEEDED( hResult );
}

HRESULT KinectWrapper::UpdateBodyData( IBody** ppBodies )
{
	HRESULT hr = E_FAIL;

	if( pBodyReader != nullptr )
	{
		hr = pBodyReader->AcquireLatestFrame( &pBodyFrame );
		if( SUCCEEDED( hr ) )
		{
			hr = pBodyFrame->GetAndRefreshBodyData( BODY_COUNT, ppBodies );
		}
	}

	return hr;
}

void KinectWrapper::updateColorFrame()
{
	IColorFrame * pColorFrame;
	HRESULT hResult = pColorReader->AcquireLatestFrame( &pColorFrame );
	if( FAILED( hResult ) ) {
		std::cerr << "Error: pColorReader->AcquireLatestFrame" << std::endl;
	}
	else {
		hResult = pColorFrame->CopyConvertedFrameDataToArray( colorBufferSize, reinterpret_cast< BYTE* >( colorBufferMat->data ), ColorImageFormat::ColorImageFormat_Bgra );
		if( SUCCEEDED( hResult ) ){
			cvtColor( *colorBufferMat, *colorMat, CV_BGR2RGB );
			colorscaleImage.setFromPixels( colorMat->data, colorWidth, colorHeight );
		}
	}

	SafeRelease( pColorFrame );
}

void KinectWrapper::updateDepthFrame()
{
	HRESULT hResult = S_OK;
	IDepthFrame * pDepthFrame = nullptr;
	hResult = pDepthReader->AcquireLatestFrame( &pDepthFrame );
	if( FAILED( hResult ) ) {
		std::cerr << "Error: pDepthReader->AcquireLatestFrame" << std::endl;
	}
	else {
		hResult = pDepthFrame->AccessUnderlyingBuffer( &this->depthBufferSize, reinterpret_cast< UINT16** >( &bufferMat->data ) );
		if( SUCCEEDED( hResult ) ){
			bufferMat->convertTo( *depthMat, CV_8U, -255.0f / 4500.0f, 255.0f );
			grayscaleImage.setFromPixels( depthMat->data, depthWidth, depthHeight );
		}
	}

	SafeRelease( pDepthFrame );
}

void KinectWrapper::updateSkeleton()
{
	IBody* ppBodies[ BODY_COUNT ] = { 0 };
	pHaveBodyData = SUCCEEDED( UpdateBodyData( ppBodies ) );
	if( pHaveBodyData ) {
		for( int i = 0; i < BODY_COUNT; ++i )
		{
			HRESULT hResult = ppBodies[ i ]->GetJoints( JointType::JointType_Count, *pJoints[ i ] );
			if( FAILED( hResult ) ){
				std::cerr << "Error: ppBodies[ i ]->GetJoints" << std::endl;
			}
			//SafeRelease( ppBodies[ i ] );
			*ppBodies[ i ] = *ppBodies[ i ];
		}
		
	}
	else {
		std::cerr << "Error: UpdateBodyData" << std::endl;
	}
}


Joint * KinectWrapper::getJointData( unsigned int bodyIndex, JointType jointType )
{
	return this->pJoints[ bodyIndex ][ jointType ];
}

void KinectWrapper::updateFace()
{
	for( int i = 0; i < BODY_COUNT; ++i ){
		HRESULT hResult = S_OK;

		hResult = pFaceReader[ i ]->AcquireLatestFrame( &pFaceFrame[ i ] );
		BOOLEAN bFaceTracked = false;

		if( SUCCEEDED( hResult ) && nullptr != pFaceFrame )
		{
			// check if a valid face is tracked in this face frame
			hResult = pFaceFrame[ i ]->get_IsTrackingIdValid( &bFaceTracked );

			if( SUCCEEDED( hResult ) )
			{
				if( bFaceTracked )
				{
					hResult = pFaceFrame[ i ]->get_FaceFrameResult( &pFaceFrameResult[ i ] );
				}
				else {
					std::cout << "Face not successfully tracked." << std::endl;
					// face tracking is not valid - attempt to fix the issue
					// a valid body is required to perform this step
					if( pHaveBodyData )
					{
						// check if the corresponding body is tracked 
						// if this is true then update the face frame source to track this body
						IBody* pBody = this->pBodies[ i ];
						if( pBody != nullptr )
						{
							BOOLEAN bTracked = false;
							hResult = pBody->get_IsTracked( &bTracked );

							UINT64 bodyTId;
							if( SUCCEEDED( hResult ) && bTracked )
							{
								// get the tracking ID of this body
								hResult = pBody->get_TrackingId( &bodyTId );
								if( SUCCEEDED( hResult ) )
								{
									// update the face frame source with the tracking ID
									pFaceSource[ i ]->put_TrackingId( bodyTId );
									pHdFaceSource[ i ]->put_TrackingId( bodyTId );
								}
								else {
									std::cerr << "getTrackingId failed." << std::endl;
								}
							}
							else {
								std::cerr << "pBody->get_IsTracked failed." << std::endl;
							}
							SafeRelease( pBody );
						}
						else {
							std::cerr << "IBody " << i << " == nullptr" << std::endl;
						}
					}
					else {
						std::cerr << "Body not updated." << std::endl;
					}
				}
			}
		}
	}
}

IFaceFrame * KinectWrapper::getFaceFrame( unsigned int faceId )
{
	return pFaceFrame[ faceId ];
}

void KinectWrapper::releaseFaceFrame( unsigned int faceId )
{
	SafeRelease( pFaceFrame[ faceId ] );
}

IFaceFrameResult * KinectWrapper::getFaceFrameResult( unsigned int faceId )
{
	return pFaceFrameResult[ faceId ];
}

void KinectWrapper::releaseFaceFrameResult( unsigned int faceId )
{
	SafeRelease( pFaceFrameResult[ faceId ] );
}

void KinectWrapper::updateHdFace()
{
	HRESULT hResult = S_OK;
	for( int i = 0; i < BODY_COUNT; i++ ){
		hResult = pHdFaceReader[ i ]->AcquireLatestFrame( &pHdFaceFrame[ i ] );
		if( SUCCEEDED( hResult ) && pHdFaceFrame[ i ] != nullptr ){
			BOOLEAN bFaceTracked = false;
			hResult = pHdFaceFrame[ i ]->get_IsFaceTracked( &bFaceTracked );
			if( SUCCEEDED( hResult ) && bFaceTracked ){
				hResult = pHdFaceFrame[ i ]->GetAndRefreshFaceAlignmentResult( pFaceAlignment[ i ] );
				if( SUCCEEDED( hResult ) && pFaceAlignment[ i ] != nullptr ){
					// Face Model Building
					if( !produce[ i ] ){
						std::system( "cls" );
						FaceModelBuilderCollectionStatus collection;
						hResult = pHdFaceModelBuilder[ i ]->get_CollectionStatus( &collection );
						if( collection == FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_Complete ){
							std::cout << "Status : Complete" << std::endl;
							IFaceModelData* pFaceModelData = nullptr;
							hResult = pHdFaceModelBuilder[ i ]->GetFaceData( &pFaceModelData );
							if( SUCCEEDED( hResult ) && pFaceModelData != nullptr ){
								hResult = pFaceModelData->ProduceFaceModel( &pFaceModel[ i ] );
								if( SUCCEEDED( hResult ) && pFaceModel[ i ] != nullptr ){
									produce[ i ] = true;
								}
							}
							SafeRelease( pFaceModelData );
						}
						/*
						else{
							std::cout << "Status : " << collection << std::endl;
							cv::putText( *colorMat, "Status : " + std::to_string( collection ), cv::Point( 50, 50 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
							// Collection Status
							if( collection >= FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_TiltedUpViewsNeeded ){
								std::cout << "Need : Tilted Up Views" << std::endl;
								cv::putText( *colorMat, "Need : Tilted Up Views", cv::Point( 50, 100 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
							}
							else if( collection >= FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_RightViewsNeeded ){
								std::cout << "Need : Right Views" << std::endl;
								cv::putText( *colorMat, "Need : Right Views", cv::Point( 50, 100 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
							}
							else if( collection >= FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_LeftViewsNeeded ){
								std::cout << "Need : Left Views" << std::endl;
								cv::putText( *colorMat, "Need : Left Views", cv::Point( 50, 100 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
							}
							else if( collection >= FaceModelBuilderCollectionStatus::FaceModelBuilderCollectionStatus_FrontViewFramesNeeded ){
								std::cout << "Need : Front ViewFrames" << std::endl;
								cv::putText( *colorMat, "Need : Front ViewFrames", cv::Point( 50, 100 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
							}
							// Capture Status
							FaceModelBuilderCaptureStatus capture;
							hResult = pHdFaceModelBuilder[ i ]->get_CaptureStatus( &capture );
							switch( capture ){
							case FaceModelBuilderCaptureStatus::FaceModelBuilderCaptureStatus_FaceTooFar:
								std::cout << "Error : Face Too Far from Camera" << std::endl;
								cv::putText( *colorMat, "Error : Face Too Far from Camera", cv::Point( 50, 150 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
								break;
							case FaceModelBuilderCaptureStatus::FaceModelBuilderCaptureStatus_FaceTooNear:
								std::cout << "Error : Face Too Near to Camera" << std::endl;
								cv::putText( *colorMat, "Error : Face Too Near to Camera", cv::Point( 50, 150 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
								break;
							case FaceModelBuilderCaptureStatus_MovingTooFast:
								std::cout << "Error : Moving Too Fast" << std::endl;
								cv::putText( *colorMat, "Error : Moving Too Fast", cv::Point( 50, 150 ), cv::FONT_HERSHEY_SIMPLEX, 1.0f, static_cast< cv::Scalar >( color[ i ] ), 2, CV_AA );
								break;
							default:
								break;
							}
						}*/
					}
					// HD Face Points
					hResult = pFaceModel[ i ]->CalculateVerticesForAlignment( pFaceAlignment[ i ], pFaceVertexCount, &pHdFacePoints[ i ][ 0 ] );
					if( FAILED( hResult ) ) {
						std::cerr << "Error: pFaceModel[ i ]->CalculateVerticesForAlignment" << std::endl;
					}
				}
			}
		}
	}
}

IHighDefinitionFaceFrame * KinectWrapper::getHdFaceFrame( unsigned int faceId )
{
	return pHdFaceFrame[ faceId ];
}

void KinectWrapper::releaseHdFaceFrame( unsigned int faceId )
{
	SafeRelease( pHdFaceFrame[ faceId ] );
}

