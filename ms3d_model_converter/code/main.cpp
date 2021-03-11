#include "_MS3DFile.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <assert.h>

#include <DirectXMath.h>

using namespace std;


struct Group
{
	int startIndex;
	int indexCount;

	int materialIndex;
};

struct Vertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct Material
{
	char textureName[ 128 ];
};

struct Frame
{
	float time;
	float px, py, pz;
	float qx, qy, qz;
};

struct Joint
{
	char jointName[ 32 ];
	float x, y, z;
	float rx, ry, rz;
	std::vector< Frame > frames;
};


vector< Group > groups;
vector< Vertex > vertices;
vector< unsigned short > indices;
vector< Material > materials;
vector< Joint > joints;


int main( int argc, char** argv )
{
	CMS3DFile model;

	//std::cout << argv[ 1 ] << std::endl;

	model.LoadFromFile( "ninja.ms3d" );

	int groupCount = model.GetNumGroups();
	
	cout << "group count: " << groupCount << endl;

	int cur = 0;

	for ( int i = 0; i < groupCount; ++i )
	{
		ms3d_group_t* group;

		model.GetGroupAt( i, &group );

		int triangleCount = group->numtriangles;

		cout << triangleCount << endl;

		Group g;

		g.startIndex = cur;
		g.indexCount = 3 * triangleCount;
		g.materialIndex = group->materialIndex;
		
		groups.push_back( g );

		for ( int j = 0; j < triangleCount; ++j )
		{
			ms3d_triangle_t* triangle;

			model.GetTriangleAt( group->triangleIndices[ j ], &triangle );

			int k = 0;
			{
				ms3d_vertex_t* vertex;

				model.GetVertexAt( triangle->vertexIndices[ k ], &vertex );

				Vertex v;

				v.x = vertex->vertex[ 0 ];
				v.y = vertex->vertex[ 1 ];
				v.z = -vertex->vertex[ 2 ];

				v.nx = triangle->vertexNormals[ k ][ 0 ];
				v.ny = triangle->vertexNormals[ k ][ 1 ];
				v.nz = triangle->vertexNormals[ k ][ 2 ];

				v.u = triangle->s[ k ];
				v.v = triangle->t[ k ];

				vertices.push_back( v );

				indices.push_back( cur++ );
			}
			k = 2;
			{
				ms3d_vertex_t* vertex;

				model.GetVertexAt( triangle->vertexIndices[ k ], &vertex );

				Vertex v;

				v.x = vertex->vertex[ 0 ];
				v.y = vertex->vertex[ 1 ];
				v.z = -vertex->vertex[ 2 ];

				v.nx = triangle->vertexNormals[ k ][ 0 ];
				v.ny = triangle->vertexNormals[ k ][ 1 ];
				v.nz = triangle->vertexNormals[ k ][ 2 ];

				v.u = triangle->s[ k ];
				v.v = triangle->t[ k ];

				vertices.push_back( v );

				indices.push_back( cur++ );
			}
			k = 1;
			{
				ms3d_vertex_t* vertex;

				model.GetVertexAt( triangle->vertexIndices[ k ], &vertex );

				Vertex v;

				v.x = vertex->vertex[ 0 ];
				v.y = vertex->vertex[ 1 ];
				v.z = -vertex->vertex[ 2 ];

				v.nx = triangle->vertexNormals[ k ][ 0 ];
				v.ny = triangle->vertexNormals[ k ][ 1 ];
				v.nz = triangle->vertexNormals[ k ][ 2 ];

				v.u = triangle->s[ k ];
				v.v = triangle->t[ k ];

				vertices.push_back( v );

				indices.push_back( cur++ );
			}
		}

		ms3d_material_t* material;

		model.GetMaterialAt( group->materialIndex, &material );

		Material m;

		string stri = material->texture;
		cout << stri << endl;

		int pos = stri.rfind( '.' );
		if ( pos != std::string::npos )
		{
			stri = stri.substr( 0, pos );

			stri += ".dds";
			cout << stri << endl;
		}

		pos = stri.rfind( '/' );
		if ( pos != std::string::npos )
		{
			stri = stri.substr( pos + 1 );

			cout << stri << endl;
		}

		memcpy( m.textureName, stri.c_str(), stri.length() + 1 );
		materials.push_back( m );
	}

	int jointCount = model.GetNumJoints();

	std::cout << "joint count: " << jointCount << std::endl;

	for ( int i = 0; i < jointCount; ++i )
	{
		ms3d_joint_t* joint;
		model.GetJointAt( i, &joint );
		
		Joint _j;

		

		std::string stri = joint->name;
		memcpy( _j.jointName, stri.c_str(), stri.length() + 1 );

		std::cout << _j.jointName << std::endl;

		assert( joint->numKeyFramesRot == joint->numKeyFramesTrans );

		std::cout << joint->numKeyFramesRot << ", " << joint->numKeyFramesTrans << std::endl;

		_j.frames.resize( joint->numKeyFramesRot );
		for ( int j = 0; j < _j.frames.size(); ++j )
		{
			assert( joint->keyFramesTrans[j].time == joint->keyFramesRot[j].time );
			_j.frames[ j ].px = joint->keyFramesTrans[j].position[ 0 ];
			_j.frames[ j ].py = joint->keyFramesTrans[j].position[ 1 ];
			_j.frames[ j ].pz = -joint->keyFramesTrans[j].position[ 2 ];

			_j.frames[ j ].qx = -joint->keyFramesRot[j].rotation[ 0 ];
			_j.frames[ j ].qy = -joint->keyFramesRot[j].rotation[ 1 ];
			_j.frames[ j ].qz = joint->keyFramesRot[j].rotation[ 2 ];

			_j.frames[ j ].time = joint->keyFramesRot[j].time;
		}

		joints.push_back( _j );
	}

	for ( int i = 0; i < indices.size(); ++i )
	{
		//cout << "( " << vertices[ i ].x << ", " << vertices[ i ].y << ", " << vertices[ i ].z << " )" << endl;
		//cout << indices[ i ] << endl;
	}

	string s = "ninja.ms3d";//argv[ 1 ];
	int pos = s.rfind( '.' );
		if ( pos != std::string::npos )
		{
			s = s.substr( 0, pos );

			s += ".mdl";
			cout << s << endl;
		}


	std::ofstream file;

	file.open( s, std::ios_base::binary );

	int _vertexCount = vertices.size();
	int _indexCount = indices.size();
	int _groupCount = groups.size();
	int _materialCount = materials.size();
	int _jointCount = joints.size();

	std::cout << _vertexCount << std::endl;
	std::cout << _indexCount << std::endl;
	std::cout << _groupCount << std::endl;
	std::cout << _materialCount << std::endl;

	file.write( ( char* )&_vertexCount, 4 );
	file.write( ( char* )&_indexCount, 4 );
	file.write( ( char* )&_groupCount, 4 );
	file.write( ( char* )&_materialCount, 4 );
	file.write( ( char* )&_jointCount, 4 );

	file.write( ( char* )&vertices[ 0 ], sizeof( Vertex ) * _vertexCount );
	file.write( ( char* )&indices[ 0 ], 2 * _indexCount );
	file.write( ( char* )&groups[ 0 ], sizeof( Group ) * _groupCount );
	file.write( ( char* )&materials[ 0 ], sizeof( Material ) * _materialCount ); // assumes 1 material !this is where da error is future meee

	for ( int i = 0; i < _jointCount; ++i )
	{
		int frameCount = joints[ i ].frames.size();
		file.write( ( char* )&frameCount, 4 );
		file.write( ( char* )&(joints[i].frames[ 0 ]), sizeof( Frame ) * frameCount );
	}

	int endInt = 12345;
	file.write( ( char* )&endInt, 4 );

	file.close();

	system( "pause" );

	return 0;
}