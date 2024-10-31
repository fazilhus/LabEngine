#version 460 core
layout(location=0) in vec3 iPos;

layout(location=0) out vec3 oCol;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	gl_Position = perspective * view * transform * vec4(iPos, 1);
	oCol = light.ambient + light.diffuse + light.specular;
}
