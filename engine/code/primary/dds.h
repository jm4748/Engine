#pragma once


#include <d3d11.h>

#include <vector>


bool CreateTexture( ID3D11Device* device,
                    std::vector< unsigned char >& dds,
                    ID3D11Resource** resource,
                    ID3D11ShaderResourceView** shaderResourceView );