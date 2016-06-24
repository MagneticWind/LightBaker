#version 150

in vec3 POSITION;
in vec3 NORMAL;

void main()
{
	gl_Position = vec4(POSITION.x, POSITION.y, POSITION.z, 1.0);
}