#pragma once


#include <d3d11.h>

#include <string>
#include <vector>


enum InputElementFormat
{
	IEF_FLOAT  = 0x29,
	IEF_FLOAT2 = 0x10,
	IEF_FLOAT3 = 0x06,
	IEF_FLOAT4 = 0x02,
};

struct InputElement
{
	std::string name;
	unsigned int index;
	InputElementFormat format;
};

class Effect
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	//----------

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	std::vector< ID3D11Buffer* > constantBuffers;

public:
	Effect( ID3D11Device* device, ID3D11DeviceContext* context );
	~Effect();

	int Create( std::vector< unsigned char >& vertexShaderCSO,
	            std::vector< unsigned char >& pixelShaderCSO,
	            std::vector< InputElement  >& inputElements,
	            std::vector< unsigned int  >& constantBufferSizes );

	void UpdateConstantBuffer( int buffer, void* data, int size );

	void Set();
};