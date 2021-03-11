#pragma comment( lib, "dxguid.lib" )


#include "dds.h"

#include "_DDSTextureLoader.cpp"


bool CreateTexture( ID3D11Device* device,
                    std::vector< unsigned char >& dds,
                    ID3D11Resource** resource,
                    ID3D11ShaderResourceView** shaderResourceView )
{
	return !FAILED( DirectX::CreateDDSTextureFromMemory( device,
	                                                     &dds[ 0 ],
	                                                     dds.size(),
	                                                     resource,
	                                                     shaderResourceView ) );
}