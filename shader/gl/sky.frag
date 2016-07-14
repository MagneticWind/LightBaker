#version 150

precision highp float;

uniform samplerCube cubemap;  // Cubemap texture sampler

in vec3 f3VShaderDir;

out vec4 fragColor;

void main()
{
	fragColor = texture(cubemap, f3VShaderDir);
}

