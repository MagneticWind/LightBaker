Texture2D    g_txDepthMap : register (t0);
Texture2D    g_txRotateMap : register(t1);
SamplerState g_samLinear : register(s0);

cbuffer CBTransformation : register(b0)
{
	matrix mWorld;
	matrix mView;
	matrix mProjection;
}

// total number of samples: 2 * NUM_SAMPLER_PAIRS + 1
#define NUM_SAMPLER_PAIRS 5

// Apply dual-radii blending to get both fine details and smooth looking
#define NUM_RADII 2

cbuffer CBSSAO : register(b1)
{
	float g_fWidth;
	float g_fHeight;
	float g_fFarPlaneZ;
	float g_fCenterWeight;

	float g_fInnerRadius;
	float g_fOuterRadius;
	float g_fMaxRadius;
	float g_fPlaceHolder;

	float4 g_f4SamplePoints[NUM_SAMPLER_PAIRS];
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
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
	output.Tex = input.Tex;
	return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
	float2 f2CurrentPixel = input.Tex.xy * float2(g_fWidth, g_fHeight);
	float fInvAspectRatio = g_fHeight / g_fWidth;

	float2 f2RotationUV = f2CurrentPixel * 0.25;
		float2 f2RotationSinCos = float2(0, 0);//g_txRotateMap.Sample( g_samLinear, f2RotationUV ).xy;
		float2x2 rotationMatrix =
	{
		{ f2RotationSinCos.y, f2RotationSinCos.x },
		{ -f2RotationSinCos.x, f2RotationSinCos.y }
	};
	float fDepthInViewSpace = g_txDepthMap.Sample(g_samLinear, input.Tex).x * g_fFarPlaneZ;

	// the occlusion of the center sample
	float fOcclusion = 0.5f * g_fCenterWeight;
	float totalOcclusion[NUM_RADII] = { fOcclusion, fOcclusion };

	float radii[NUM_RADII] = { g_fInnerRadius, g_fOuterRadius };
	float fFinalOcclusion = 0.f;
	for (int i = 0; i < NUM_RADII; ++i)
	{
		float fRadius = radii[i] / fDepthInViewSpace;
		fRadius = min(fRadius, g_fMaxRadius);

		for (int j = 0; j < NUM_SAMPLER_PAIRS; ++j)
		{
			float4 f4SamplePoint = g_f4SamplePoints[j];
				float2 f2SamplePoint = f4SamplePoint.xy; //mul(f4SamplePoint.xy, rotationMatrix);
				//f2SamplePoint *= fInvAspectRatio;
				float2 f2SampleOffset = f2SamplePoint * fRadius; // z component is the line width
				float fLineWidth = f4SamplePoint.z * fRadius;  // linewidth is Zs

			float2 f2UVSample1 = input.Tex + f2SampleOffset;
				float fDepth1 = g_txDepthMap.Sample(g_samLinear, f2UVSample1).x;
			float2 f2UVSample2 = input.Tex - f2SampleOffset;
				float fDepth2 = g_txDepthMap.Sample(g_samLinear, f2UVSample2).x;

			float2 f2SamplesDepthInViewSpace = float2(fDepth1, fDepth2) * g_fFarPlaneZ;
				float2 f2SamplesDepthDifferences = fDepthInViewSpace.xx - f2SamplesDepthInViewSpace; // dr

				float2 f2SamplesOcclusion = saturate((f2SamplesDepthDifferences / fLineWidth) + 0.5f); // max(min(Zs, dr) + zs, 0) / (2*Zs)

				f2SamplesOcclusion *= f4SamplePoint.w; // line weight

			// add occlusion of the paired samples
			totalOcclusion[i] += f2SamplesOcclusion.x;
			totalOcclusion[i] += f2SamplesOcclusion.y;
		}
		totalOcclusion[i] = saturate((totalOcclusion[i] - 0.5f) * 2.f);
		fFinalOcclusion += totalOcclusion[i];
	}

	//float fDepth = g_txDepthMap.Sample( g_samLinear, input.Tex).x;
	//return float4(fDepth, fDepth, fDepth, 1.f);
	return float4(1 - fFinalOcclusion, 1 - fFinalOcclusion, 1 - fFinalOcclusion, 0.1);
	//return float4(fFinalOcclusion, fFinalOcclusion, fFinalOcclusion, 1.f);
	//return float4(0.5, 0.5, 0.5, 0.5);
}