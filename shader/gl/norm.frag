#version 150

precision highp float;

out vec4 fragColor;

layout (std140) uniform CBuffer1
{
    vec4 f4Ka;
    vec4 f4Kd;
    vec4 f4Ks;
    vec4 f4Ke;
};

layout (std140) uniform CBuffer2
{
	vec4 f4LightDirection;
	vec4 f4LightDirectionalColor;
	vec4 f4LightPosition[3];
	vec4 f4LightColor[3];
};

in vec4 f4VShaderPos;
in vec4 f4VShaderNorm;
in vec4 f4VShaderView;

//--------------------------------------------------------------------------------------
// lighting functions for different light sources
//--------------------------------------------------------------------------------------
vec4 DirectionalLight(vec4 f4Normal, vec4 f4View, vec4 f4LightDirection, vec4 f4LightColor)
{
	vec4 f4Color = vec4(0,0,0,0);

	// ambient parts
	f4Color += f4Ka * f4LightColor;

	// diffuse part
	float NDotL = clamp(dot(normalize(f4Normal.xyz), normalize(f4LightDirection.xyz)), 0, 1);
	f4Color += f4Kd * f4LightColor * NDotL;

	// glossy part
	//    float3 f3ViewDir = normalize(f4View.xyz);// view direction
	//    float3 f3HalfVec = normalize(f3ViewDir + f4LightDirection.xyz);        // half vector
	//    float NDotH = saturate(dot(f3HalfVec, f4Normal.xyz));
	//    f4Color += f4Ks * f4LightColor * pow(NDotH, f4Ks.w);

	return f4Color;
}

void main()
{
    vec4 f4FinalColor = vec4(0, 0, 0, 0);

	f4FinalColor += DirectionalLight(f4VShaderNorm, f4VShaderView, f4LightDirection, f4LightDirectionalColor);

	//f4FinalColor += PointLight(input.f4Norm, input.f4View, input.f4Pos);
    
    // texture color
    // convert texture color from gamma space to linear space
    //f4FinalColor *= pow(txColor.Sample( samLinear, input.f2Tex ), 2.2f);

	fragColor = f4FinalColor;
}