
cbuffer CBTransformation : register(b0)
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
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Pos = mul(input.Pos, mWorld);
	output.Pos = mul(output.Pos, mView);
	output.Pos = mul(output.Pos, mProjection);

	return output;
}
