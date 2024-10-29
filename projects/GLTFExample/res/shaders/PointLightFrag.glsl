#version 460 core
layout(location=0) in vec2 iUV;

uniform sampler2D gPos;
uniform sampler2D gDiffSpec;
uniform sampler2D gNorm;

// struct DirectionalLight {
// 	vec3 dir;

// 	vec3 ambient;
// 	vec3 diffuse;
// 	vec3 specular;
// };

// uniform DirectionalLight dlight;

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
};

uniform PointLight light;

// struct SpotLight {
// 	vec3 pos;
// 	vec3 dir;
// 	float cutoff
// 	float outerCutoff;

// 	vec3 ambient;
// 	vec3 diffuse;
// 	vec3 specular;

// 	vec3 attenuation;
// };

// uniform SpotLight slights[MAX_NUM_LIGHT_SOURCES];
// uniform int slights_count;

uniform vec3 cam_pos;

out vec4 oColor;

// vec3 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 cam_dir, vec3 diff, float spec);
vec3 CalcPointLight(vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec);
// vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec);

void main()
{
	
	vec3 pos = texture(gPos, iUV).xyz;
	vec3 norm = texture(gNorm, iUV).rgb;
	vec3 diff = texture(gDiffSpec, iUV).rgb;
	float spec = texture(gDiffSpec, iUV).a;

	vec3 cam_dir = normalize(cam_pos - pos);

	// Global Light
	// vec3 tempCol = CalcDirectionalLight(dlight, norm, cam_dir, diff, spec);

	// Point Lights
	vec3 tempCol = CalcPointLight(norm, cam_dir, pos, diff, spec);

	// Spot Lights
	// for (int i = 0; i < slights_count; i++)
	// {
	// 	tempCol += CalcSpotLight(slights[i], norm, cam_dir, pos, diff, spec);
	// }

	float gamma = 0.9;
	tempCol = pow(tempCol, vec3(1.0 / gamma));
	oColor = vec4(tempCol, 1.0);
	//oColor = vec4(pos, 1.0);
}

// vec3 CalcDirectionalLight(DirectionalLight light, vec3 norm, vec3 cam_dir, vec3 diff, float spec)
// {
// 	vec3 ambient = light.ambient * diff;

// 	vec3 ldir = normalize(-light.dir);
// 	float d = max(dot(norm, ldir), 0.0);
// 	vec3 diffuse = light.diffuse * d * diff;

// 	vec3 halfwaydir = normalize(ldir + cam_dir);
// 	float s = pow(max(dot(norm, -halfwaydir), d), spec * 128.0);
// 	vec3 specular = light.specular * spec * diff;

// 	return ambient + diffuse + specular;
// }

vec3 CalcPointLight(vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec)
{
	vec3 ambient = light.ambient * diff;

	vec3 ldir = normalize(light.pos - frag_pos);
	float d = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * d * diff;

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float s = pow(max(dot(norm, -halfwaydir), d), spec * 128.0);
	vec3 specular = light.specular * spec * diff;

	float dist = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
	return (ambient + diffuse + specular) * attenuation;
}

// vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec)
// {
// 	vec3 ambient = light.ambient * diff;
	
// 	vec3 ldir = normalize(light.pos - frag_pos);
// 	float d = max(dot(norm, ldir), 0.0);
// 	vec3 diffuse = light.diffuse * d * diff;

// 	vec3 halfwaydir = normalize(ldir + cam_dir);
// 	float s = pow(max(dot(norm, -halfwaydir), d), spec * 128.0);
// 	vec3 specular = light.specular * spec * diff;

// 	float theta = dot(ldir, normalize(-light.dir));
// 	float epsilon = light.cutoff - light.outerCutoff;
// 	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

// 	float dist = length(light.pos - frag_pos);
// 	float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
// 	return (ambient + (diffuse + specular) * intensity) * attenuation;
// }