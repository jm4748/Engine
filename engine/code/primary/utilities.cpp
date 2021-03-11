#include "utilities.h"

#include <fstream>


bool LoadFile( std::string fileName, std::vector< unsigned char >& fileData )
{
	std::ifstream file;

	file.open( fileName, std::ifstream::binary );

	if ( !file.is_open() )
	{
		return false;
	}

	file.seekg( 0, std::ifstream::end );
	fileData.resize( ( unsigned )file.tellg() );
	file.seekg( 0, std::ifstream::beg );

	file.read( ( char* )&fileData[ 0 ], fileData.size() );

	file.close();

	return true;
}


float Rand( float min, float max )
{
	return min + ( ( float )rand() / ( float )RAND_MAX ) * ( max - min );
}