  
//--------------------------------------------------------------------------------------
// constant buffer varialbes
//--------------------------------------------------------------------------------------
cbuffer CBTransform : register( b0 )
{
    matrix mWorld;
    matrix mView;
    matrix mProjection;
}

cbuffer CBMaterial : register( b1 )
{
    float4 f4Ka;
    float4 f4Kd;
    float4 f4Ks;
    float4 f4Ke;
}

cbuffer CBLighting : register( b2 )
{
    float4 f4SunDirection;
    float4 f4SunColor;
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
};

//--------------------------------------------------------------------------------------
// lighting functions for different light sources
//--------------------------------------------------------------------------------------
float4 DirectionalLight(in float4 f4Normal, in float4 f4View)
{
    float4 f4Color = 0;
    
    // ambient part
    f4Color += f4Ka * f4SunColor;

    // diffuse part
    float NDotL = saturate( dot (normalize(f4Normal.xyz), normalize(f4SunDirection.xyz) ) );
    f4Color += f4Kd * f4SunColor * NDotL;

    // glossy part
    float3 f3ViewDir = normalize(f4View.xyz);// view direction
    float3 f3HalfVec = normalize(f3ViewDir + f4SunDirection.xyz);        // half vector
    float NDotH = saturate(dot(f3HalfVec, f4Normal.xyz));
    f4Color += f4Ks * f4SunColor * pow(NDotH, f4Ks.w);
    
    return f4Color;
}


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;

    // world transformation
    output.f4Pos   = mul( input.f4Pos, mWorld );
    output.f4Norm = input.f4Norm;   //@TODO: use normal transformation
	output.f4View.xyz = normalize(mView[3].xyz - output.f4Pos.xyz);

    // view&projection transformation
    output.f4SVPos = mul( output.f4Pos, mView );    // everything in world space
    output.f4SVPos = mul( output.f4SVPos, mProjection);

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( PS_INPUT input) : SV_Target
{
    float4 f4FinalColor = 0;

    f4FinalColor += DirectionalLight(input.f4Norm, input.f4View);
    
    // texture color
    // convert texture color from gamma space to linear space
    //f4FinalColor *= pow(txColor.Sample( samLinear, input.f2Tex ), 2.2f);
    
    return f4FinalColor;
}