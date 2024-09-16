#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iUV;
layout(location=0) out vec4 oColor;
layout(location=1) out vec2 oUV;
uniform mat4 t;
uniform mat4 view;
uniform mat4 persp;
void main()
{
	gl_Position = persp * view * t * vec4(iPos, 1);
	oColor = vec4(0.0);
	oUV = iUV;
}
