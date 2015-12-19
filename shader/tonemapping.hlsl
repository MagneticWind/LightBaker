Texture2D tx2D : register( t0 );

cbuffer CBTransformation : register( b0 )
{
    matrix mWorld;
    matrix mView;
    matrix mProjection;
}

cbuffer CBCommon : register( b1 )
{
    float4 param; // (witdth, height, avg_lumimance)
}

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
float3 ReinhardTonemapping(in float3 f3Color)
{
    float3 f3ResultColor = f3Color / ( f3Color + 1 );
    return f3ResultColor;
}

//--------------------------------------------------------------------------------------
float3 JimhejlTonemapping(in float3 f3Color)
{
    float3 f3ResultColor = max(0, f3Color - 0.004);
    f3ResultColor = (f3ResultColor * (6.2 * f3ResultColor + 0.5)) / (f3ResultColor * (6.2 * f3ResultColor + 1.7) + 0.06);
    return f3ResultColor;
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, mWorld );
    output.Pos = mul( output.Pos, mView );
    output.Pos = mul( output.Pos, mProjection );
    output.Tex = input.Tex;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    static const float  MIDDLE_GRAY = 0.72f;
    static const float  LUM_WHITE = 1.5f;
    
    float4 f4Color = tx2D.Load(float3(param.x * input.Tex.x, param.y * input.Tex.y, 0.f));
    
    f4Color.rgb *= MIDDLE_GRAY / (param.z + 0.001f);
    f4Color.rgb *= (1.0f + f4Color.rgb / LUM_WHITE);
    
	f4Color.rgb = JimhejlTonemapping(f4Color.rgb);
    
    // return gamma corrected color
    return pow(abs(f4Color), 1.f/2.2f);
    //return f4Color;
}