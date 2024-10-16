#version 460 core
layout(location=0) in vec3 pos;
layout(location=0) out vec4 color;
uniform mat4 transform;
void main()
{
	gl_Position = transform * vec4(pos, 1);
	color = vec4(pos.x, 0, pos.y, 1);
}
