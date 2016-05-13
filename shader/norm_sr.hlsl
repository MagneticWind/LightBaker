Texture2D    g_txShadowMap : register (t0);
SamplerState g_samLinear : register(s0);

//--------------------------------------------------------------------------------------
// constant buffer varialbes
//--------------------------------------------------------------------------------------
#define MAX_CASCADE_COUNT 4

#define DEBUG_CASCADES 0

cbuffer CBTransform : register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
}

cbuffer CBMaterial : register(b1)
{
	float4 f4Ka;
	float4 f4Kd;
	float4 f4Ks;
	float4 f4Ke;
}

cbuffer CBLighting : register(b2)
{
	float4 f4LightDirection;
	float4 f4LightDirectionalColor;
	float4 f4LightPosition[3];
	float4 f4LightColor[3];
}

cbuffer CBShadow : register(b3)
{
	matrix mShadowProjection[MAX_CASCADE_COUNT];
	matrix mShadowView;
	float4 f4DepthRange;
}

//--------------------------------------------------------------------------------------
// shader input layout
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 f4Pos  : POSITION;
	float4 f4Norm : NORMAL;
};

struct PS_INPUT
{
	float4 f4SVPos : SV_POSITION;
	float4 f4Norm  : NORMAL;
	float4 f4Pos   : TEXCOORD0;
	float4 f4View  : TEXCOORD1;
	float4 f4ViewSpacePos : TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// lighting functions for different light sources
//--------------------------------------------------------------------------------------
float4 DirectionalLight(in float4 f4Normal, in float4 f4View, in float4 f4LightDirection, in float4 f4LightColor)
{
    float4 f4Color = 0;
    
    // ambient part
    f4Color += f4Ka * f4LightColor;

    // diffuse part
	float NDotL = saturate(dot(normalize(f4Normal.xyz), normalize(f4LightDirection.xyz)));
    f4Color += f4Kd * f4LightColor * NDotL;

    // glossy part
//    float3 f3ViewDir = normalize(f4View.xyz);// view direction
//    float3 f3HalfVec = normalize(f3ViewDir + f4LightDirection.xyz);        // half vector
//    float NDotH = saturate(dot(f3HalfVec, f4Normal.xyz));
//    f4Color += f4Ks * f4LightColor * pow(NDotH, f4Ks.w);
    
    return f4Color;
}

float4 PointLight(in float4 f4Normal, in float4 f4View, in float4 f4Position, in float4 f4LightPosition, in float4 f4LightColor)
{
	float4 f4Color = 0;

	float3 f3LightDir = f4LightPosition.xyz - f4Position.xyz;
	float fDistance = sqrt(dot(f3LightDir, f3LightDir));

	f3LightDir /= fDistance;

	// ambient part
	f4Color += f4Ka * f4LightColor;

	// diffuse part
	float NDotL = saturate(dot(normalize(f4Normal.xyz), f3LightDir));
	f4Color += f4Kd * f4LightColor * NDotL;

	// glossy part
	//float3 f3ViewDir = normalize(f4View.xyz);// view direction
	//	float3 f3HalfVec = normalize(f3ViewDir + f3LightDir);        // half vector
	//float NDotH = saturate(dot(f3HalfVec, f4Normal.xyz));
	//f4Color += f4Ks * f4LightColor * pow(NDotH, f4Ks.w);

	float fadeFactor = 1.f / (fDistance);

	return f4Color * fadeFactor;
}

float4 ComputeShadowFactor(in float4 worldSpacePos, in float4 viewSpacePos)
{
	// select cascade based on the distance
	int cascadeIndex = 0;
	if (viewSpacePos.z < f4DepthRange.x)
	{
		cascadeIndex = 0;
#if DEBUG_CASCADES
		return float4(1, 0, 0, 0);
#endif
	}
	else if (viewSpacePos.z < f4DepthRange.y)
	{
		cascadeIndex = 1;
#if DEBUG_CASCADES
		return float4(0, 1, 0, 0);
#endif
	}
	else if (viewSpacePos.z < f4DepthRange.z)
	{
		cascadeIndex = 2;
#if DEBUG_CASCADES
		return float4(0, 0, 1, 0);
#endif
	}
	else
	{
		cascadeIndex = 3;
#if DEBUG_CASCADES
		return float4(1, 1, 0, 0);
#endif
	}

	// project world position in light space
	float4 pos = mul(worldSpacePos, mShadowView);
	pos = mul(pos, mShadowProjection[cascadeIndex]);

	// uv in cascaded shadow map
	float2 uv = pos.xy;
	uv = uv * 0.5f + float2(0.5, 0.5);
	uv.y = 1 - uv.y;
	uv.y = uv.y * 0.25 + cascadeIndex * 0.25;

	// depth
	float depthInMap = g_txShadowMap.Sample(g_samLinear, uv).x;

	float factor = (pos.z / pos.w) < depthInMap;
	return float4(factor, factor, factor, factor);

}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;

	// world transformation
	output.f4Pos = mul(input.f4Pos, mWorld);
	output.f4Norm = input.f4Norm;   //@TODO: use normal transformation
	output.f4View.xyz = normalize(mView[3].xyz - output.f4Pos.xyz);

	// view&projection transformation
	output.f4SVPos = mul(output.f4Pos, mView);
	output.f4ViewSpacePos = output.f4SVPos;
	output.f4ViewSpacePos.z /= output.f4ViewSpacePos.w;
	output.f4SVPos = mul(output.f4SVPos, mProjection);

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float4 f4FinalColor = 0;

	// compute a shader factor
	// find a cascade base on z value in view space, and compare with depth range
	float4 shadowFactor = ComputeShadowFactor(input.f4Pos, input.f4ViewSpacePos);

	f4FinalColor += DirectionalLight(input.f4Norm, input.f4View, f4LightDirection, f4LightDirectionalColor) * shadowFactor;

//	for (int i = 0; i < 3; ++i)
//		f4FinalColor += PointLight(input.f4Norm, input.f4View, input.f4Pos, f4LightPosition[i], f4LightColor[i]);

	// texture color
	// convert texture color from gamma space to linear space
	//f4FinalColor *= pow(txColor.Sample( samLinear, input.f2Tex ), 2.2f);

	return f4FinalColor;
}