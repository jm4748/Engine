#pragma once


#include <d3d11.h>

#include "dds.h"


class Texture
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	//----------

	ID3D11Texture2D* texture;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;
	D3D11_VIEWPORT viewport;

	int width, height;

public:
	Texture( ID3D11Device* device, ID3D11DeviceContext* context );
	~Texture();

	bool Create( std::vector< unsigned char >& dds );
	bool Create( int width, int height, bool dsbavp );

	void SetSR( int slot );
	void SetRT( ID3D11DepthStencilView* depthStencilView );

	void Clear( float* color );
};