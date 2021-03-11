struct PSInput //VSOutput
{
	float4 svPosition    : SV_POSITION;
	float2 textureCoords : TEXCOORD;
};


Texture2D tex    : register( t0 );
SamplerState sam : register( s0 );


float4 main( PSInput input ) : SV_TARGET
{
	float4 color = tex.Sample( sam, input.textureCoords );

	return color;
}