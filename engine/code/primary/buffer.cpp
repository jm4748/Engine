#include "buffer.h"


Buffer::Buffer( ID3D11Device* device, ID3D11DeviceContext* context )
{
	this->device = device;
	this->context = context;

	//----------

	type = BT_VERTEX;

	largeIndices = false;
	
	buffer = nullptr;
}

Buffer::~Buffer()
{
	if ( buffer != nullptr ) buffer->Release();
}


bool Buffer::Create( BufferType bufferType, UsageType usageType, int size, void* data, bool largeIndices )
{
	type = bufferType;

	this->largeIndices = largeIndices;

	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.ByteWidth           = size;
	bufferDesc.Usage               = ( D3D11_USAGE )usageType;
	bufferDesc.BindFlags           = bufferType;
	bufferDesc.CPUAccessFlags      = ( usageType == UT_DYNAMIC ) ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags           = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA bufferData;

	bufferData.pSysMem = data;

	if ( FAILED( device->CreateBuffer( &bufferDesc, ( data != nullptr ) ? &bufferData : nullptr, &buffer ) ) ) return false;

	return true;
}


void Buffer::Update( void* data, int size )
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	context->Map( buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource );
	CopyMemory( mappedSubresource.pData, data, size );
	context->Unmap( buffer, 0 );
}


void Buffer::Set( int stride )
{
	if ( type == BT_VERTEX )
	{
		UINT offset = 0;

		context->IASetVertexBuffers( 0, 1, &buffer, ( UINT* )&stride, &offset );
	}
	else
	{
		if ( largeIndices ) context->IASetIndexBuffer( buffer, DXGI_FORMAT_R32_UINT, 0 );
		else context->IASetIndexBuffer( buffer, DXGI_FORMAT_R16_UINT, 0 );
	}
}