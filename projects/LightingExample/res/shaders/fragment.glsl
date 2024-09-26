#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

struct DirectionalLight {
	vec3 dir;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionalLight dlight;

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};

uniform PointLight plight;

uniform vec3 cam_pos;
uniform sampler2D tex; 

out vec4 oColor;

void main()
{
	//Global Light
	vec3 ambient = dlight.ambient * material.ambient;

	vec3 norm = normalize(iNorm);
	vec3 ldir = normalize(-dlight.dir);
	float diff = max(dot(norm, ldir), 0.0);
	vec3 diffuse = dlight.diffuse * (diff * material.diffuse);

	vec3 camdir = normalize(cam_pos - iPos);
	vec3 halfwaydir = normalize(ldir + camdir);
	float spec = pow(max(dot(norm, halfwaydir), diff), 128.0 * material.shininess);
	vec3 spec_term = dlight.specular * (spec * material.specular);

	// Point Lights
	ambient += plight.ambient * material.ambient;

	ldir = normalize(plight.pos - iPos);
	diff = max(dot(norm, ldir), 0.0);
	diffuse += plight.diffuse * (diff * material.diffuse);

	halfwaydir = normalize(ldir + camdir);
	spec = pow(max(dot(norm, halfwaydir), diff), 128.0 * material.shininess);
	spec_term += plight.specular * (spec * material.specular);

	float dist = length(plight.pos - iPos);
	float attenuation = 1.0 / (plight.attenuation.x + plight.attenuation.y * dist + plight.attenuation.z * (dist * dist));
	vec3 tcolor = ambient + diffuse + spec;
	oColor = vec4(tcolor, 1.0);
}
