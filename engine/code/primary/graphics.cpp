#include "graphics.h"
#include "utilities.h"
#include <iostream>


template< typename T >
int Graphics::FindEmptySlot( T t )
{
	for ( unsigned int i = 0; i < t.size(); ++i )
	{
		if ( t[ i ] == nullptr )
		{
			return i;
		}
	}

	return -1;
}


Graphics::Graphics()
{
	hWnd = 0;

	device = nullptr;
	context = nullptr;
	swapChain = nullptr;

	renderTargetView = nullptr;

	depthStencilBuffer = nullptr;
	depthStencilView = nullptr;

	effects.resize( maxEffectCount, nullptr );
	buffers.resize( maxBufferCount, nullptr );
	textures.resize( maxTextureCount, nullptr );
}


bool Graphics::Startup( int width, int height, HWND hWnd )
{
	this->hWnd = hWnd;

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	swapChainDesc.BufferDesc.Width                   = width;
	swapChainDesc.BufferDesc.Height                  = height;
	swapChainDesc.BufferDesc.RefreshRate.Numerator   = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count                   = 1;
	swapChainDesc.SampleDesc.Quality                 = 0;
	swapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount                        = 1;
	swapChainDesc.OutputWindow                       = hWnd;
	swapChainDesc.Windowed                           = true;
	swapChainDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags                              = 0;

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT result;

	result = D3D11CreateDeviceAndSwapChain( nullptr,
	                                        D3D_DRIVER_TYPE_HARDWARE,
	                                        0,
	                                        flags,
	                                        nullptr,
	                                        0,
	                                        D3D11_SDK_VERSION,
	                                        &swapChainDesc,
	                                        &swapChain,
	                                        &device,
	                                        &featureLevel,
	                                        &context );

	if ( FAILED( result ) ) return false;

	if ( featureLevel < D3D_FEATURE_LEVEL_10_0 )
	{
		MessageBox( 0, "Feature Level Unsupported", nullptr, MB_OK );

		return false;
	}

	//----------
	std::vector< unsigned char > vertexShaderCSO, pixelShaderCSO;

	LoadFile( "..\\x64\\Debug\\simple_vs.cso", vertexShaderCSO );
	LoadFile( "..\\x64\\Debug\\simple_ps.cso", pixelShaderCSO );

	std::vector< InputElement > inputElements =
	{
		{ "POSITION", 0, IEF_FLOAT3 },
		{ "TEXCOORD", 0, IEF_FLOAT2 },
	};

	std::vector< unsigned int > constantBufferSizes;

	simpleEffect = CreateEffect( vertexShaderCSO, pixelShaderCSO, inputElements, constantBufferSizes );

	D3D11_SAMPLER_DESC samplerDesc;

	ZeroMemory( &samplerDesc, sizeof( D3D11_SAMPLER_DESC ) );
 
	samplerDesc.Filter           = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU         = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV         = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW         = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias       = 0.0f;
	samplerDesc.MaxAnisotropy    = 1;
	samplerDesc.ComparisonFunc   = D3D11_COMPARISON_NEVER;
	samplerDesc.BorderColor[ 0 ] = 1.0f;
	samplerDesc.BorderColor[ 1 ] = 1.0f;
	samplerDesc.BorderColor[ 2 ] = 1.0f;
	samplerDesc.BorderColor[ 3 ] = 1.0f;
	samplerDesc.MinLOD           = -FLT_MAX;
	samplerDesc.MaxLOD           = +FLT_MAX;

	device->CreateSamplerState( &samplerDesc, &samplerState );
	
	std::vector< unsigned short > indices;

	indices.push_back( 0 );
	indices.push_back( 1 );
	indices.push_back( 2 );
	indices.push_back( 2 );
	indices.push_back( 3 );
	indices.push_back( 0 );

	simpleIndexBuffer = CreateBuffer( BT_INDEX, UT_IMMUTABLE, sizeof( unsigned short ) * ( unsigned )indices.size(), &indices[ 0 ] );
	//----------

	return true;
}

void Graphics::Cleanup()
{
	samplerState->Release();
	DestroyBuffer( simpleIndexBuffer );
	DestroyEffect( simpleEffect );

	for ( auto i : textures ) if ( i != nullptr ) delete i;
	for ( auto i : buffers  ) if ( i != nullptr ) delete i;
	for ( auto i : effects  ) if ( i != nullptr ) delete i;

	if ( swapChain != nullptr ) swapChain->SetFullscreenState( false, nullptr );
	if ( context != nullptr ) context->ClearState();

	if ( renderTargetView != nullptr ) renderTargetView->Release();

	if ( depthStencilView != nullptr ) depthStencilView->Release();
	if ( depthStencilBuffer != nullptr ) depthStencilBuffer->Release();

	if ( device != nullptr ) device->Release();
	if ( context != nullptr ) context->Release();
	if ( swapChain != nullptr ) swapChain->Release();
}


void Graphics::ResizeBackBuffer( int width, int height )
{
	context->OMSetRenderTargets( 0, nullptr, nullptr );

	if ( renderTargetView != nullptr ) renderTargetView->Release();

	swapChain->ResizeBuffers( 0, width, height, DXGI_FORMAT_UNKNOWN, 0 );

	ID3D11Texture2D* backBuffer;

	swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( void** )&backBuffer );
	device->CreateRenderTargetView( backBuffer, nullptr, &renderTargetView );
	backBuffer->Release();

	if ( depthStencilView != nullptr ) depthStencilView->Release();
	if ( depthStencilBuffer != nullptr ) depthStencilBuffer->Release();

	D3D11_TEXTURE2D_DESC depthStenciDesc;

	depthStenciDesc.Width              = width;
	depthStenciDesc.Height             = height;
	depthStenciDesc.MipLevels          = 1;
	depthStenciDesc.ArraySize          = 1;
	depthStenciDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStenciDesc.SampleDesc.Count   = 1;
	depthStenciDesc.SampleDesc.Quality = 0;
	depthStenciDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthStenciDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthStenciDesc.CPUAccessFlags     = 0;
	depthStenciDesc.MiscFlags          = 0;

	device->CreateTexture2D( &depthStenciDesc, nullptr, &depthStencilBuffer );

	device->CreateDepthStencilView( depthStencilBuffer, nullptr, &depthStencilView );

	context->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width    = ( float )width;
	viewport.Height   = ( float )height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	context->RSSetViewports( 1, &viewport );

	context->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}


void Graphics::Clear( float r, float g, float b, float a )
{
	float color[ 4 ] = { r, g, b, a };

	context->ClearRenderTargetView( renderTargetView, color );
	context->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}

void Graphics::Present()
{
	swapChain->Present( 0, 0 );
}


int Graphics::CreateEffect( std::vector< unsigned char >& vertexShaderCSO,
                            std::vector< unsigned char >& pixelShaderCSO,
                            std::vector< InputElement  >& inputElements,
                            std::vector< unsigned int  >& constantBufferSizes )
{
	int i = FindEmptySlot( effects );

	if ( i == -1 ) return -1;

	Effect* effect = new Effect( device, context );

	int error = effect->Create( vertexShaderCSO, pixelShaderCSO, inputElements, constantBufferSizes );

	if ( error != 0 ) return error;

	effects[ i ] = effect;

	return i;
}

void Graphics::DestroyEffect( int effect )
{
	if ( effects[ effect ] != nullptr )
	{
		delete effects[ effect ];
		effects[ effect ] = nullptr;
	}
}


void Graphics::UpdateConstantBuffer( int effect, int buffer, void* data, int size )
{
	effects[ effect ]->UpdateConstantBuffer( buffer, data, size );
}


void Graphics::SetEffect( int effect )
{
	effects[ effect ]->Set();
}


int Graphics::CreateBuffer( BufferType bufferType, UsageType usageType, int size, void* data )
{
	int i = FindEmptySlot( buffers );

	if ( i == -1 ) return -1;

	Buffer* buffer = new Buffer( device, context );

	if ( !buffer->Create( bufferType, usageType, size, data ) ) return -2;

	buffers[ i ] = buffer;

	return i;
}

void Graphics::DestroyBuffer( int buffer )
{
	if ( buffers[ buffer ] != nullptr )
	{
		delete buffers[ buffer ];
		buffers[ buffer ] = nullptr;
	}
}


void Graphics::UpdateBuffer( int buffer, void* data, int size )
{
	buffers[ buffer ]->Update( data, size );
}


void Graphics::SetBuffer( int buffer, int stride )
{
	buffers[ buffer ]->Set( stride );
}


void Graphics::DrawIndexed( int start, int count )
{
	context->DrawIndexed( count, start, 0 );
}

void Graphics::Draw( int start, int count )
{
	context->Draw( count, start );
}


int Graphics::CreateTexture( std::vector< unsigned char >& dds )
{
	int i = FindEmptySlot( textures );

	if ( i == -1 ) return -1;

	Texture* texture = new Texture( device, context );

	if ( !texture->Create( dds ) ) return -2;

	textures[ i ] = texture;

	return i;
}

int Graphics::CreateTexture( int width, int height, bool dsbavp )
{
	int i = FindEmptySlot( textures );

	if ( i == -1 ) return -1;

	Texture* texture = new Texture( device, context );

	if ( !texture->Create( width, height, dsbavp ) ) return -2;

	textures[ i ] = texture;

	return i;
}

void Graphics::DestroyTexture( int texture )
{
	delete textures[ texture ];
	textures[ texture ] = nullptr;
}


void Graphics::SetTexture( int texture, int slot ) //PS
{
	textures[ texture ]->SetSR( slot );
}


void Graphics::SetRenderTarget( int texture )
{
	if ( texture == -1 )
	{
		context->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );
		context->RSSetViewports( 1, &viewport );
	}
	else
	{
		textures[ texture ]->SetRT( depthStencilView );
	}
}

void Graphics::ClearRenderTarget( int texture, float r, float g, float b, float a )
{
	float color[ 4 ] = { r, g, b, a };

	textures[ texture ]->Clear( color );
}


void Graphics::DrawTexture( int texture, DirectX::XMFLOAT4 rectangle )
{
	SetEffect( simpleEffect );

	std::vector< SimpleVertex > vertices;

	vertices.push_back( SimpleVertex( DirectX::XMFLOAT3( rectangle.x, rectangle.y, 0.0f ), DirectX::XMFLOAT2( 0.0f, 0.0f ) ) );
	vertices.push_back( SimpleVertex( DirectX::XMFLOAT3( rectangle.z, rectangle.y, 0.0f ), DirectX::XMFLOAT2( 1.0f, 0.0f ) ) );
	vertices.push_back( SimpleVertex( DirectX::XMFLOAT3( rectangle.z, rectangle.w, 0.0f ), DirectX::XMFLOAT2( 1.0f, 1.0f ) ) );
	vertices.push_back( SimpleVertex( DirectX::XMFLOAT3( rectangle.x, rectangle.w, 0.0f ), DirectX::XMFLOAT2( 0.0f, 1.0f ) ) );

	simpleVertexBuffer = CreateBuffer( BT_VERTEX, UT_IMMUTABLE, sizeof( SimpleVertex ) * ( unsigned )vertices.size(), &vertices[ 0 ] );

	SetBuffer( simpleVertexBuffer, sizeof( SimpleVertex ) );

	SetBuffer( simpleIndexBuffer );

	SetTexture( texture, 0 );
	context->PSSetSamplers( 0, 1, &samplerState );

	DrawIndexed( 0, 6 );

	DestroyBuffer( simpleVertexBuffer );
}


HWND Graphics::GetHWnd()
{
	return hWnd;
}


ID3D11Device* Graphics::GetDevice()
{
	return device;
}

ID3D11DeviceContext* Graphics::GetContext()
{
	return context;
}


ID3D11RenderTargetView** Graphics::GetRenderTargetView()
{
	return &renderTargetView;
}

ID3D11DepthStencilView* Graphics::GetDepthStencilView()
{
	return depthStencilView;
}