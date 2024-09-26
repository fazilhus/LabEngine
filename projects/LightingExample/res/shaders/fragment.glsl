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
	//vec4 ambient_col = vec4(0.1, 0.1, 0.1, 1.0);
	//vec4 surf_col = texture(tex, iUV);
	//vec4 ambient_term = ambient_col * light_col;
	//
	//vec3 norm = normalize(iNorm);
	//vec3 ldir = normalize(light_pos - iPos);
	//float diff = max(dot(norm, ldir), 0.0);
	//vec4 diffuse_term = light_col * diff;
	//
	//vec3 camdir = normalize(cam_pos - iPos);
	//vec3 halfwaydir = normalize(ldir + camdir);
	//float spec = pow(max(dot(norm, halfwaydir), diff), light_int);
	//vec4 spec_term = spec * light_col;
	//
	//float dist = length(light_pos - iPos);
	//float attenuation = 1.0 / (light_att.x + light_att.y * dist + light_att.z * (dist * dist));
	//oColor = (ambient_term + diffuse_term + spec_term) * surf_col;
	//oColor = vec4(oColor.rgb * attenuation, oColor.a);

	vec3 ambient = plight.ambient * material.ambient;

	vec3 norm = normalize(iNorm);
	vec3 ldir = normalize(plight.pos - iPos);
	float diff = max(dot(norm, ldir), 0.0);
	vec3 diffuse = plight.diffuse * (diff * material.diffuse);

	vec3 camdir = normalize(cam_pos - iPos);
	vec3 halfwaydir = normalize(ldir + camdir);
	float spec = pow(max(dot(norm, halfwaydir), diff), 128.0 * material.shininess);
	vec3 spec_term = plight.specular * (spec * material.specular);

	float dist = length(plight.pos - iPos);
	float attenuation = 1.0 / (plight.attenuation.x + plight.attenuation.y * dist + plight.attenuation.z * (dist * dist));
	vec3 tcolor = ambient + diffuse + spec;
	oColor = vec4(tcolor, 1.0);
}
