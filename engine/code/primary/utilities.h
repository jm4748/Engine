#pragma once


#include <string>
#include <vector>


bool LoadFile( std::string fileName, std::vector< unsigned char >& fileData );


struct _Buffer
{
	std::vector< unsigned char > data;
	unsigned i = 0;


	template< typename T >
	T Read()
	{
		T t = *( ( T* )&data[ i ] );

		i += sizeof( T );

		return t;
	}

	template< typename T >
	void Read( void* dest, unsigned count )
	{
		unsigned size = sizeof( T ) * count;

		memcpy( dest, &data[ i ], size );

		i += size;
	}
};


float Rand( float min, float max ); //should this be here?