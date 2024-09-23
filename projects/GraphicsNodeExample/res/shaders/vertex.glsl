#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;
layout(location=0) out vec3 oPos;
layout(location=1) out vec3 oNorm;
layout(location=2) out vec2 oUV;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;
void main()
{
	gl_Position = perspective * view * transform * vec4(iPos, 1);
	oPos = (transform * vec4(iPos, 1.0)).xyz;
	oNorm = iNorm;
	oUV = iUV;
}
