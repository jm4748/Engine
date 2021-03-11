#pragma once


#include <DirectXMath.h>

#include "..\primary\graphics.h"
#include "..\primary\utilities.h"

#include "texture_manager.h"

#include <iostream>
#include <vector>


using namespace DirectX;


class Model
{
	friend class ModelEffect;

public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoords;


		Vertex()
		{
		}

		Vertex( XMFLOAT3 position )
		{
			this->position = position;
		}
	};

	struct Group
	{
		int startIndex;
		int indexCount;

		int materialIndex;
	};

	struct Material
	{
		char textureNameArray[ 128 ];
	};

	struct Material2
	{
		std::string textureName;
		int textureIndex;
	};

	struct Frame
	{
		float time;
		XMFLOAT3 position;
		XMFLOAT3 angles;
	};

	struct Joint
	{
		std::vector< Frame > frames;
	};

private:
	std::vector< Vertex > vertices;
	std::vector< unsigned short > indices;
	std::vector< Group > groups;
	std::vector< Material > materials;
	std::vector< Material2 > materials2;
	std::vector< Joint > joints;

	int vertexBuffer;
	int indexBuffer;

public:
	Model();

	bool Load( Graphics* graphics, const char* fileName );
	void Unload( Graphics* graphics );
};


class ModelEffect
{
private:
	struct PerFrame
	{
		XMFLOAT4X4 vp;
	};

	struct PerObject
	{
		XMFLOAT4X4 w;
	};

private:
	Graphics* graphics;

	TextureManager* textureManager;

	int effect;

	PerFrame perFrame;
	int perFrameBuffer;
	PerObject perObject;
	int perObjectBuffer;

	Model* model;

	ID3D11SamplerState* samplerState;

public:
	ModelEffect();

	bool Initialize( Graphics* graphics, TextureManager* textureManager );
	void Release();

	void Apply();

	void SetModel( Model* model );

	void SetVP( XMFLOAT4X4* vp );
	void SetW( XMFLOAT4X4* w );

	void Draw();
};