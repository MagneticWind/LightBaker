#version 150

in vec3 POSITION;
in vec3 NORMAL;

layout (std140) uniform CBuffer0
{
	mat4 mWorld;
	mat4 mView;
	mat4 mProjection;
};

out vec3 f3VShaderDir;

void main()
{
	vec4 f4VShaderPos = vec4(POSITION.x, POSITION.y, POSITION.z, 1.0);
	
	f3VShaderDir = normalize(POSITION);

	gl_Position = mProjection * mView * mWorld * f4VShaderPos;
}