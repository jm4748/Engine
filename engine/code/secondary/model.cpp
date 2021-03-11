#include "model.h"

#include <iostream>


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


Model::Model()
{
	vertexBuffer = -1;
	indexBuffer = -1;
}


bool Model::Load( Graphics* graphics, const char* fileName )
{
	//std::cout << "loading model" << std::endl;

	//-----

	_Buffer buffer;

	LoadFile( fileName, buffer.data );

	vertices.resize( buffer.Read< unsigned >() );
	indices.resize( buffer.Read< unsigned >() );
	groups.resize( buffer.Read< unsigned >() );
	materials.resize( buffer.Read< unsigned >() );
	joints.resize( buffer.Read< unsigned >() );

	materials2.resize( materials.size() );

	
	std::cout << "vertices:   " << vertices.size()   << std::endl;
	std::cout << "indices:    " << indices.size()    << std::endl;
	std::cout << "groups:     " << groups.size()     << std::endl;
	std::cout << "materials:  " << materials.size()  << std::endl;
	std::cout << "materials2: " << materials2.size() << std::endl;
	std::cout << "joints:     " << joints.size()     << std::endl;
	

	buffer.Read< Vertex >( &vertices[ 0 ], ( unsigned )vertices.size() );
	buffer.Read< unsigned short >( &indices[ 0 ], ( unsigned )indices.size() );
	buffer.Read< Group >( &groups[ 0 ], ( unsigned )groups.size() );
	buffer.Read< Material >( &materials[ 0 ], ( unsigned )materials.size() );

	for ( unsigned i = 0; i < joints.size(); ++i )
	{
		joints[ i ].frames.resize( buffer.Read< unsigned >() );

		//std::cout << "joint " << i << ", frames: " << joints[ i ].frames.size() << std::endl;

		buffer.Read< Frame >( &( joints[ i ].frames[ 0 ] ), ( unsigned )joints[ i ].frames.size() );
	}

	int end = buffer.Read< unsigned >();
	if ( end == 12345 )
	{
		//std::cout << "end reached" << std::endl;
	}

	//-----

	vertexBuffer = graphics->CreateBuffer( BT_VERTEX,
	                                       UT_IMMUTABLE,
	                                       sizeof( Vertex ) * ( unsigned )vertices.size(),
	                                       &vertices[ 0 ] );

	indexBuffer = graphics->CreateBuffer( BT_INDEX,
	                                      UT_IMMUTABLE,
	                                      sizeof( unsigned short ) * ( unsigned )indices.size(),
	                                      &indices[ 0 ] );

	for ( unsigned i = 0; i < materials.size(); ++i )
	{
		std::string textureName = "resources\\";

		textureName += materials[ i ].textureNameArray;

		std::vector< unsigned char > data;

		if ( !LoadFile( textureName.c_str(), data ) ) std::cout << "error" << std::endl;


		
		materials2[ i ].textureName = textureName;
		materials2[ i ].textureIndex = graphics->CreateTexture( data );

		
		std::cout << "i: " << i << std::endl;
		std::cout << "name: "  << materials2[ i ].textureName  << std::endl;
		std::cout << "index: " << materials2[ i ].textureIndex << std::endl;
		
	}

	return true;
}

void Model::Unload( Graphics* graphics )
{
	std::cout << "unloading model" << std::endl;

	for ( unsigned i = 0; i < materials2.size(); ++i )
	{
		std::cout << materials2[ i ].textureName << std::endl;
		//graphics->GetTextureManager()->DestroyTexture( materials2[ i ].textureIndex );
	}

	vertices.clear();
	indices.clear();
	groups.clear();
	materials.clear();
	materials2.clear();
	joints.clear();
}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


ModelEffect::ModelEffect()
{
	effect = -1;
}


bool ModelEffect::Initialize( Graphics* graphics, TextureManager* textureManager )
{
	this->graphics = graphics;

	std::vector< unsigned char > vertexShaderCSO, pixelShaderCSO;

	LoadFile( "..\\x64\\Debug\\model_vs.cso", vertexShaderCSO );
	LoadFile( "..\\x64\\Debug\\model_ps.cso", pixelShaderCSO );

	std::vector< InputElement > inputElements =
	{
		{ "POSITION", 0, IEF_FLOAT3 },
		{ "NORMAL",   0, IEF_FLOAT3 },
		{ "TEXCOORD", 0, IEF_FLOAT2 },
	};

	std::vector< unsigned int > constantBufferSizes = { sizeof( PerFrame ), sizeof( PerObject ) };

	perFrameBuffer = 0;
	perObjectBuffer = 1;

	effect = graphics->CreateEffect( vertexShaderCSO, pixelShaderCSO, inputElements, constantBufferSizes );

	if ( effect < 0 )
	{
		std::cout << "error: " << effect << std::endl;
	}

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

	graphics->GetDevice()->CreateSamplerState( &samplerDesc, &samplerState );

	return true;
}

void ModelEffect::Release()
{
}


void ModelEffect::Apply()
{
	graphics->SetEffect( effect );

	graphics->GetContext()->PSSetSamplers( 0, 1, &samplerState );
}


void ModelEffect::SetModel( Model* model )
{
	this->model = model;

	graphics->SetBuffer( model->vertexBuffer, sizeof( Model::Vertex ) );
	graphics->SetBuffer( model->indexBuffer );
}


void ModelEffect::SetVP( XMFLOAT4X4* vp )
{
	XMStoreFloat4x4( &perFrame.vp, XMMatrixTranspose( XMLoadFloat4x4( vp ) ) );
	graphics->UpdateConstantBuffer( effect, perFrameBuffer, &perFrame, sizeof( PerFrame ) );
}

void ModelEffect::SetW( XMFLOAT4X4* w )
{
	XMStoreFloat4x4( &perObject.w, XMMatrixTranspose( XMLoadFloat4x4( w ) ) );
	graphics->UpdateConstantBuffer( effect, perObjectBuffer, &perObject, sizeof( PerObject ) );
}


void ModelEffect::Draw()
{
	for ( unsigned int i = 0; i < model->groups.size(); ++i )
	{
		int textureIndex = model->materials2[ model->groups[ i ].materialIndex ].textureIndex;

		graphics->SetTexture( textureIndex, 0 );

		graphics->DrawIndexed( model->groups[ i ].startIndex, model->groups[ i ].indexCount );
	}
}