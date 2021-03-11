#include "effect.h"


Effect::Effect( ID3D11Device* device, ID3D11DeviceContext* context )
{
	this->device = device;
	this->context = context;

	//----------

	vertexShader = nullptr;
	pixelShader = nullptr;

	inputLayout = nullptr;
}

Effect::~Effect()
{
	if ( vertexShader != nullptr ) vertexShader->Release();
	if ( pixelShader != nullptr ) pixelShader->Release();

	if ( inputLayout != nullptr ) inputLayout->Release();

	for ( auto i : constantBuffers ) if ( i != nullptr ) i->Release();
}


int Effect::Create( std::vector< unsigned char >& vertexShaderCSO,
                    std::vector< unsigned char >& pixelShaderCSO,
                    std::vector< InputElement  >& inputElements,
                    std::vector< unsigned int  >& constantBufferSizes )
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > inputElementDescs;

	for ( auto& i : inputElements )
	{
		D3D11_INPUT_ELEMENT_DESC inputElementDesc;

		inputElementDesc.SemanticName         = i.name.c_str();
		inputElementDesc.SemanticIndex        = i.index;
		inputElementDesc.Format               = ( DXGI_FORMAT )i.format;
		inputElementDesc.InputSlot            = 0;
		inputElementDesc.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		inputElementDesc.InstanceDataStepRate = 0;

		inputElementDescs.push_back( inputElementDesc );
	}

	if ( FAILED( device->CreateVertexShader( &vertexShaderCSO[ 0 ], ( unsigned )vertexShaderCSO.size(), NULL, &vertexShader ) ) )
		return -2;

	if ( FAILED( device->CreateInputLayout( &inputElementDescs[ 0 ],
	                                        ( unsigned )inputElementDescs.size(),
	                                        &vertexShaderCSO[ 0 ],
	                                        ( unsigned )vertexShaderCSO.size(),
	                                        &inputLayout ) ) )
		return -3;

	if ( FAILED( device->CreatePixelShader( &pixelShaderCSO[ 0 ], ( unsigned )pixelShaderCSO.size(), NULL, &pixelShader ) ) )
		return -4;

	for ( auto i : constantBufferSizes )
	{
		D3D11_BUFFER_DESC bufferDesc;

		bufferDesc.ByteWidth           = i;
		bufferDesc.Usage               = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags      = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags           = 0;
		bufferDesc.StructureByteStride = 0;

		ID3D11Buffer* buffer = nullptr;

		if ( FAILED( device->CreateBuffer( &bufferDesc, NULL, &buffer ) ) ) return -5;

		constantBuffers.push_back( buffer );
	}

	return 0;
}


void Effect::UpdateConstantBuffer( int buffer, void* data, int size )
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;

	context->Map( constantBuffers[ buffer ], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource );
	CopyMemory( mappedSubresource.pData, data, size );
	context->Unmap( constantBuffers[ buffer ], 0 );
}


void Effect::Set()
{
	context->VSSetShader( vertexShader, NULL, 0 );
	context->PSSetShader( pixelShader, NULL, 0 );

	context->IASetInputLayout( inputLayout );

	if ( constantBuffers.size() > 0 ) context->VSSetConstantBuffers( 0, ( unsigned )constantBuffers.size(), &constantBuffers[ 0 ] );
}