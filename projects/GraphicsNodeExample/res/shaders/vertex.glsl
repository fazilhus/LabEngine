#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iUV;
layout(location=0) out vec2 oUV;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;
void main()
{
	gl_Position = perspective * view * transform * vec4(iPos, 1);
	oUV = iUV;
}
