#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iUV;

layout(location=0) out vec2 oUV;

void main()
{
	gl_Position = vec4(iPos, 1);
	oUV = iUV;
}
