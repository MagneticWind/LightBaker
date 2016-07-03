#version 150

in vec3 POSITION;
in vec3 NORMAL;

layout (std140) uniform CBuffer0
{
	mat4 mWorld;
	mat4 mView;
	mat4 mProjection;
};

out vec4 f4VShaderPos;
out vec4 f4VShaderNorm;
out vec4 f4VShaderView;

void main()
{

	f4VShaderPos = mWorld * vec4(POSITION.x, POSITION.y, POSITION.z, 1.0);
	f4VShaderNorm = vec4(NORMAL.x, NORMAL.y, NORMAL.z, 1.0);
	f4VShaderView = mView[3];

	gl_Position = mProjection * mView * f4VShaderPos;


}