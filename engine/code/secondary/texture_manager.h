#pragma once


#include "..\primary\graphics.h"

#include <vector>
#include <string>
#include <map>

#include <iostream>


class TextureManager
{
private:
	struct TextureContainer
	{
		int texture;
		int referenceCount;


		TextureContainer()
		{
			texture = -1;
			referenceCount = 0;
		}
	};

private:
	Graphics* graphics;

	std::vector< TextureContainer > textureContainers;
	std::map< std::string, int > textureNameMap;

private:
	int FindEmptySlot()
	{
		for ( unsigned i = 0; i < textureContainers.size(); ++i )
		{
			if ( textureContainers[ i ].texture == -1 )
			{
				return i;
			}
		}

		return -1;
	}

public:
	void Startup( Graphics* graphics )
	{
		this->graphics = graphics;
		textureContainers.resize( 100 );
	}

	void Cleanup()
	{

	}


	int FindTexture( std::string textureName )
	{
		std::map< std::string, int >::iterator it;

		it = textureNameMap.find( textureName );

		if ( it == textureNameMap.end() )
		{
			return -1;
		}

		return it->second;
	}


	int CreateTextureFromMemory( std::string textureName, std::vector< unsigned char >& textureData )
	{
		int textureIndex = FindTexture( textureName );

		if ( textureIndex != -1 )
		{
			++textureContainers[ textureIndex ].referenceCount;
			std::cout << "texture index " << textureIndex << ", ref count: " << textureContainers[ textureIndex ].referenceCount << std::endl;
			return textureIndex;
		}

		textureIndex = FindEmptySlot();

		if ( textureIndex == -1 )
		{
			return -1;
		}

		
		textureContainers[ textureIndex ].texture = graphics->CreateTexture( textureData );
		textureContainers[ textureIndex ].referenceCount = 1;

		textureNameMap[ textureName ] = textureIndex;

		std::cout << "texture created, index: " << textureIndex << std::endl;

		return textureIndex;
	}

	void DestroyTexture( int textureIndex )
	{
		if ( textureContainers[ textureIndex ].texture != -1 )
		{
			if ( ( --textureContainers[ textureIndex ].referenceCount ) == 0 )
			{
				graphics->DestroyTexture( textureContainers[ textureIndex ].texture );
				textureContainers[ textureIndex ].texture = -1;
				std::cout << "texture destroyed" << std::endl;
			}
		}
	}
};