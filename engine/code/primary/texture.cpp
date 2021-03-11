#include "texture.h"


Texture::Texture( ID3D11Device* device, ID3D11DeviceContext* context )
{
	this->device = device;
	this->context = context;

	//----------

	texture = nullptr;
	renderTargetView = nullptr;
	shaderResourceView = nullptr;

	depthStencilBuffer = nullptr;
	depthStencilView = nullptr;

	width = -1;
	height = -1;
}

Texture::~Texture()
{
	if ( depthStencilView != nullptr ) depthStencilView->Release();
	if ( depthStencilBuffer != nullptr ) depthStencilBuffer->Release();

	if ( shaderResourceView != nullptr ) shaderResourceView->Release();
	if ( renderTargetView != nullptr ) renderTargetView->Release();
	if ( texture != nullptr ) texture->Release();
}


bool Texture::Create( std::vector< unsigned char >& dds )
{
	if ( CreateTexture( device, dds, ( ID3D11Resource** )&texture, &shaderResourceView ) )
	{
		D3D11_TEXTURE2D_DESC textureDesc;

		texture->GetDesc( &textureDesc );

		width = textureDesc.Width;
		height = textureDesc.Height;

		return true;
	}

	return false;
}

bool Texture::Create( int width, int height, bool dsbavp )
{
	this->width = width;
	this->height = height;

	D3D11_TEXTURE2D_DESC textureDesc;

	textureDesc.Width              = width;
	textureDesc.Height             = height;
	textureDesc.MipLevels          = 1;
	textureDesc.ArraySize          = 1;
	textureDesc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count   = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage              = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags          = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags     = 0;
	textureDesc.MiscFlags          = 0;

	if ( FAILED( device->CreateTexture2D( &textureDesc, nullptr, &texture ) ) ) return false;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	renderTargetViewDesc.Format             = textureDesc.Format;
	renderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	if ( FAILED( device->CreateRenderTargetView( texture, &renderTargetViewDesc, &renderTargetView ) ) ) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;

	shaderResourceViewDesc.Format                    = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels       = 1;

	if ( FAILED( device->CreateShaderResourceView( texture, &shaderResourceViewDesc, &shaderResourceView ) ) ) return false;

	if ( dsbavp ) //depth buffer and viewport
	{
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

		if ( FAILED( device->CreateTexture2D( &depthStenciDesc, NULL, &depthStencilBuffer ) ) ) return false;

		if ( FAILED( device->CreateDepthStencilView( depthStencilBuffer, NULL, &depthStencilView ) ) ) return false;

		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width    = ( float )width;
		viewport.Height   = ( float )height;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	}

	return true;
}


void Texture::SetSR( int slot )
{
	context->PSSetShaderResources( slot, 1, &shaderResourceView );
}

void Texture::SetRT( ID3D11DepthStencilView* depthStencilView )
{
	context->OMSetRenderTargets( 1, &renderTargetView, ( ( this->depthStencilView != nullptr ) ? ( this->depthStencilView ) : ( depthStencilView ) ) );
	context->RSSetViewports( 1, &viewport );
}


void Texture::Clear( float* color )
{
	context->ClearRenderTargetView( renderTargetView, color );
	if ( depthStencilView != nullptr ) context->ClearDepthStencilView( depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 );
}