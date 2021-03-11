struct Vertex
{
	float3 position      : POSITION;
	float2 textureCoords : TEXCOORD;
};

struct PSInput //VSOutput
{
	float4 svPosition    : SV_POSITION;
	float2 textureCoords : TEXCOORD;
};


PSInput main( Vertex vertex )
{
	PSInput output;

	output.svPosition    = float4( vertex.position, 1.0f );
	output.textureCoords = vertex.textureCoords;

	return output;
}