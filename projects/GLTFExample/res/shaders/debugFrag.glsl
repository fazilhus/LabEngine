#version 460 core
layout(location=0) in vec3 iCol;

out vec4 oColor;

void main()
{
	oColor = vec4(iCol, 1.0);
}