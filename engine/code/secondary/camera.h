#pragma once


#include <DirectXMath.h>


using namespace DirectX;


#undef near
#undef far


class Camera
{
private:
	XMFLOAT4X4 projection;

	XMFLOAT3 lastPosition, currentPosition;
	XMFLOAT4 lastOrientation, currentOrientation;

	float yaw, pitch;

public:
	Camera();

	void SetLens( float fov, float aspect, float near, float far );

	void Update( XMFLOAT3* deltaPosition, float deltaYaw, float deltaPitch );

	void GetViewProjection( float interp, XMFLOAT4X4* vp );
	void GetPosition( float interp, XMFLOAT3* position );
	void GetOrientation( float interp, XMFLOAT4X4* orientation );

	void GetLook( float interp, XMFLOAT3* look );
	void GetRight( float interp, XMFLOAT3* right );

	void SetPosition( XMFLOAT3* position );
	void SetOrientation( float yaw, float pitch );
};