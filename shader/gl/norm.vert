#version 150

in vec3 POSITION;
in vec3 NORMAL;

layout (std140) uniform CBuffer0
{
	mat4 mWorld;
	mat4 mView;
	mat4 mProjection;
};

void main()
{
	gl_Position = vec4(POSITION.x, POSITION.y, POSITION.z, 1.0);
}