#version 460 core
layout(location=0) in vec3 pos;
layout(location=0) out vec4 color;
uniform mat4 mvp;
void main()
{
	gl_Position = mvp * vec4(pos, 1);
	color = vec4(1.0, 1.0, 1.0, 1.0);
}
