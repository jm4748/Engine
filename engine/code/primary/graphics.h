#pragma once


#include <d3d11.h>
#include <DirectXMath.h>

#include "effect.h"
#include "buffer.h"
#include "texture.h"

#include <vector>


#pragma comment( lib, "d3d11.lib" )


class Graphics
{
private:
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 textureCoordinates;


		SimpleVertex( DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 textureCoordinates )
		{
			this->position = position;
			this->textureCoordinates = textureCoordinates;
		}
	};

private:
	HWND hWnd;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapChain;

	ID3D11RenderTargetView* renderTargetView;

	ID3D11Texture2D* depthStencilBuffer;
	ID3D11DepthStencilView* depthStencilView;

	D3D11_VIEWPORT viewport;

	//----------

	const int maxEffectCount  = 100;
	const int maxBufferCount  = 100;
	const int maxTextureCount = 100;

	std::vector< Effect* > effects;
	std::vector< Buffer* > buffers;
	std::vector< Texture* > textures;

	int simpleEffect;
	int simpleVertexBuffer;
	int simpleIndexBuffer;

	ID3D11SamplerState* samplerState;

private:
	template< typename T >
	int FindEmptySlot( T t );

public:
	Graphics();

	bool Startup( int width, int height, HWND hWnd );
	void Cleanup();

	void ResizeBackBuffer( int width, int height );

	void Clear( float r, float g, float b, float a );
	void Present();

	//----------

	int CreateEffect( std::vector< unsigned char >& vertexShaderCSO,
	                  std::vector< unsigned char >& pixelShaderCSO,
	                  std::vector< InputElement  >& inputElements,
	                  std::vector< unsigned int  >& constantBufferSizes );
	void DestroyEffect( int effect );

	void UpdateConstantBuffer( int effect, int buffer, void* data, int size );

	void SetEffect( int effect );

	//----------

	int CreateBuffer( BufferType bufferType, UsageType usageType, int size, void* data );
	void DestroyBuffer( int buffer );

	void UpdateBuffer( int buffer, void* data, int size );

	void SetBuffer( int buffer, int stride = -1 );

	//----------

	int CreateTexture( std::vector< unsigned char >& dds );
	int CreateTexture( int width, int height, bool dsbavp = false );
	void DestroyTexture( int texture );

	void SetTexture( int texture, int slot );

	//----------

	void DrawIndexed( int start, int count );
	void Draw( int start, int count );

	//----------

	void SetRenderTarget( int texture = -1 );
	void ClearRenderTarget( int texture, float r, float g, float b, float a );

	//----------

	void DrawTexture( int texture, DirectX::XMFLOAT4 rectangle );

	//----------

	HWND GetHWnd();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();

	ID3D11RenderTargetView** GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
};