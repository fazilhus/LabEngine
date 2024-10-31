#version 460 core
layout(location=0) in vec3 iPos;

// uniform mat4 transform;
// uniform mat4 view;
// uniform mat4 perspective;

uniform mat4 mvp;

void main()
{
	gl_Position = mvp * vec4(iPos, 1);
}
