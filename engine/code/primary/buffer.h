#pragma once


#include <d3d11.h>


enum BufferType
{
	BT_VERTEX = 0x01,
	BT_INDEX  = 0x02,
};

enum UsageType
{
	UT_IMMUTABLE = 0x01,
	UT_DYNAMIC   = 0x02,
};

class Buffer
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;

	//----------

	BufferType type;

	bool largeIndices;

	ID3D11Buffer* buffer;

public:
	Buffer( ID3D11Device* device, ID3D11DeviceContext* context );
	~Buffer();

	bool Create( BufferType bufferType, UsageType usageType, int size, void* data, bool largeIndices = false );

	void Update( void* data, int size );

	void Set( int stride );
};