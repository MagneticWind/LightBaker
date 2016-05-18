
TextureCube cubeMap : register( t0 );
SamplerState linearSampler : register( s0 );

cbuffer CBTransformation : register( b0 )
{
    matrix mWorld;
    matrix mView;
    matrix mProjection;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
	float4 Norm : NORMAL;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Dir : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
	float4 pos = mul(input.Pos, mWorld);
	pos = mul(pos, mView);

	output.Dir = normalize(pos.xyz);// - mView[3].xyz);

	pos = mul(pos, mProjection);

	output.Pos = input.Pos;
	
    
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
	float4 color = cubeMap.Sample(linearSampler, normalize(input.Dir));
    return color;
}
