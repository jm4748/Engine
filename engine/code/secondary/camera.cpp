#include "camera.h"


Camera::Camera()
{
	XMStoreFloat4x4( &projection, XMMatrixIdentity() );

	lastPosition = currentPosition = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMStoreFloat4( &lastOrientation, XMQuaternionRotationRollPitchYaw( 0.0f, 0.0f, 0.0f ) );
	currentOrientation = lastOrientation;

	yaw = 0.0f;
	pitch = 0.0f;
}


void Camera::SetLens( float fov, float aspectRatio, float near, float far )
{
	XMStoreFloat4x4( &projection, XMMatrixPerspectiveFovLH( XMConvertToRadians( fov ), aspectRatio, near, far ) );
}


void Camera::Update( XMFLOAT3* deltaPosition, float deltaYaw, float deltaPitch )
{
	lastPosition = currentPosition;
	lastOrientation = currentOrientation;

	XMStoreFloat3( &currentPosition, XMLoadFloat3( &currentPosition ) + XMLoadFloat3( deltaPosition ) );

	yaw += deltaYaw;
	pitch += deltaPitch;
	
	if ( pitch > 89.0f )
	{
		pitch = 89.0f;
	}
	else if ( pitch < -89.0f )
	{
		pitch = -89.0f;
	}

	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw( XMConvertToRadians( pitch ), XMConvertToRadians( yaw ), 0.0f );

	XMStoreFloat4( &currentOrientation, rotation );
}


void Camera::GetViewProjection( float interp, XMFLOAT4X4* vp )
{
	XMFLOAT4X4 view;

	XMStoreFloat4x4( &view, XMMatrixTranspose( XMMatrixRotationQuaternion( XMQuaternionSlerp( XMLoadFloat4( &lastOrientation ), XMLoadFloat4( &currentOrientation ), interp ) ) ) );

	XMVECTOR lastPosition = XMLoadFloat3( &( this->lastPosition ) );
	XMVECTOR currentPosition = XMLoadFloat3( &( this->currentPosition ) );
	XMVECTOR position = lastPosition + ( currentPosition - lastPosition ) * interp;

	view( 0, 3 ) = 0.0f;
	view( 1, 3 ) = 0.0f;
	view( 2, 3 ) = 0.0f;
	view( 3, 3 ) = 1.0f;

	view( 3, 0 ) = -XMVectorGetX( XMVector3Dot( XMVectorSet( view( 0, 0 ), view( 1, 0 ), view( 2, 0 ), 0.0f ), position ) );
	view( 3, 1 ) = -XMVectorGetX( XMVector3Dot( XMVectorSet( view( 0, 1 ), view( 1, 1 ), view( 2, 1 ), 0.0f ), position ) );
	view( 3, 2 ) = -XMVectorGetX( XMVector3Dot( XMVectorSet( view( 0, 2 ), view( 1, 2 ), view( 2, 2 ), 0.0f ), position ) );

	XMStoreFloat4x4( vp, XMLoadFloat4x4( &view ) * XMLoadFloat4x4( &projection ) );
}

void Camera::GetPosition( float interp, XMFLOAT3* position )
{
	XMVECTOR lastPosition = XMLoadFloat3( &( this->lastPosition ) );
	XMVECTOR currentPosition = XMLoadFloat3( &( this->currentPosition ) );

	XMStoreFloat3( position, lastPosition + ( currentPosition - lastPosition ) * interp );
}

void Camera::GetOrientation( float interp, XMFLOAT4X4* orientation )
{
	XMStoreFloat4x4( orientation, XMMatrixRotationQuaternion( XMQuaternionSlerp( XMLoadFloat4( &lastOrientation ), XMLoadFloat4( &currentOrientation ), interp ) ) );
}


void Camera::GetLook( float interp, XMFLOAT3* look )
{
	XMVECTOR q = XMQuaternionSlerp( XMLoadFloat4( &lastOrientation ), XMLoadFloat4( &currentOrientation ), interp );

	XMStoreFloat3( look, XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), q ) );
}

void Camera::GetRight( float interp, XMFLOAT3* right )
{
	XMVECTOR q = XMQuaternionSlerp( XMLoadFloat4( &lastOrientation ), XMLoadFloat4( &currentOrientation ), interp );

	XMStoreFloat3( right, XMVector3Cross( XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ), XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), q ) ) );
}


void Camera::SetPosition( XMFLOAT3* position )
{
	currentPosition = *position;
}

void Camera::SetOrientation( float yaw, float pitch )
{
	this->yaw = yaw;
	this->pitch = pitch;

	XMFLOAT3 deltaPosition( 0.0f, 0.0f, 0.0f );

	Update( &deltaPosition, 0.0f, 0.0f );
}