#version 460 core
layout(location=0) in vec4 iColor;
layout(location=1) in vec2 iUV;
layout(location=0) uniform sampler2D tex; 
out vec4 oColor;
void main()
{
	oColor = texture(tex, iUV);
}
