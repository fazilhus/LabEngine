#version 460 core
layout(location=0) in vec2 iUV;
uniform sampler2D tex; 
out vec4 oColor;
void main()
{
	oColor = texture(tex, iUV);
}
