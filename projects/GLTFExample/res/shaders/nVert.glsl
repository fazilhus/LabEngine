#version 460 core
layout(location=0) in vec3 iPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * transform * vec4(iPos, 1);
}
