struct Vertex
{
	float3 position      : POSITION;
	float3 normal        : NORMAL;
	float2 textureCoords : TEXCOORD;
};

struct PSInput //VSOutput
{
	float4 svPosition    : SV_POSITION;
	float2 textureCoords : TEXCOORD;
};


cbuffer PerFrame : register( b0 )
{
	matrix vp;
};

cbuffer PerObject : register( b1 )
{
	matrix w;
};


PSInput main( Vertex vertex )
{
	PSInput output;

	output.svPosition    = mul( float4( vertex.position, 1.0f ), mul( w, vp ) );
	output.textureCoords = vertex.textureCoords;

	return output;
}