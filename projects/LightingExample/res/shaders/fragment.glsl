#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
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

#define MAX_NUM_LIGHT_SOURCES 4

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};

uniform PointLight plights[MAX_NUM_LIGHT_SOURCES];
uniform int plights_count;

struct SpotLight {
	vec3 pos;
	vec3 dir;
	float cutoff;
	float outerCutoff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};

uniform SpotLight slights[MAX_NUM_LIGHT_SOURCES];
uniform int slights_count;

uniform vec3 cam_pos;

out vec4 oColor;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 cam_dir);
vec3 CalcPointLight(PointLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos);

void main()
{
	vec3 norm = normalize(iNorm);
	vec3 cam_dir = normalize(cam_pos - iPos);
	// Global Light
	vec3 tempCol = CalcDirectionalLight(dlight, norm, cam_dir);

	// Point Lights
	for (int i = 0; i < plights_count; i++)
	{
		tempCol += tempCol + CalcPointLight(plights[i], norm, cam_dir, iPos);
	}

	// Spot Lights
	for (int i = 0; i < slights_count; i++)
	{
		tempCol += CalcSpotLight(slights[i], norm, cam_dir, iPos);
	}

	oColor = vec4(tempCol, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 cam_dir)
{
	vec3 ambient = light.ambient * texture(material.diffuse, iUV).rgb;

	vec3 ldir = normalize(-light.dir);
	float diff = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, iUV).rgb;

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float spec = pow(max(dot(norm, halfwaydir), diff), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, iUV).rgb;

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos)
{
	vec3 ambient = light.ambient * texture(material.diffuse, iUV).rgb;

	vec3 ldir = normalize(light.pos - frag_pos);
	float diff = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, iUV).rgb);

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float spec = pow(max(dot(norm, halfwaydir), diff), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, iUV).rgb);

	float dist = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos)
{
	vec3 ambient = light.ambient * texture(material.diffuse, iUV).rgb;

	vec3 ldir = normalize(light.pos - frag_pos);
	float diff = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, iUV).rgb);

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float spec = pow(max(dot(norm, halfwaydir), diff), material.shininess);
	vec3 specular = light.specular * (spec * texture(material.specular, iUV).rgb);

	float theta = dot(ldir, normalize(-light.dir));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

	float dist = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
	return (ambient + (diffuse + specular) * intensity) * attenuation;
}