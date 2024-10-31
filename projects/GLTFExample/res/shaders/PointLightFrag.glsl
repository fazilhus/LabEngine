#version 460 core
uniform sampler2D gPos;
uniform sampler2D gCol;
uniform sampler2D gNorm;

uniform vec2 screen_dim;
uniform vec3 cam_pos;

struct PointLight {
	vec3 pos;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float radius;
	float falloff;
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

layout(location=0) out vec4 oColor;

vec3 CalcPointLight(vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec);
// vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec);

float CalcAttenuation(float dist, float rad, float falloff);
vec2 CalcUV();

void main()
{
	vec2 uv = CalcUV();
	vec3 pos = texture(gPos, uv).xyz;
	vec3 norm = texture(gNorm, uv).xyz;
	vec4 temp = texture(gCol, uv);
	vec3 diff = temp.rgb;
	float spec = temp.a;

	vec3 cam_dir = normalize(cam_pos - pos);

	// Point Light
	vec3 tempCol = CalcPointLight(norm, cam_dir, pos, diff, spec);

	// Spot Lights
	// for (int i = 0; i < slights_count; i++)
	// {
	// 	tempCol += CalcSpotLight(slights[i], norm, cam_dir, pos, diff, spec);
	// }

	float gamma = 0.9;
	tempCol = pow(tempCol, vec3(1.0 / gamma));
	oColor = vec4(tempCol, 1.0);
}

vec3 CalcPointLight(vec3 norm, vec3 cam_dir, vec3 frag_pos, vec3 diff, float spec)
{
	vec3 ambient = light.ambient * diff;

	vec3 ldir = normalize(light.pos - frag_pos);
	float d = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * d * diff;

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float s = pow(max(dot(norm, -halfwaydir), d), spec * 128.0);
	vec3 specular = light.specular * s * diff;

	float dist = length(light.pos - frag_pos);
	float attenuation = CalcAttenuation(dist, light.radius, light.falloff);
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

float CalcAttenuation(float dist, float rad, float falloff)
{
	float s = dist / rad;
	if (s >= 1.0)
		return 0.0;

	float s2 = s * s;
	return 2 * (1 - s2) * (1 - s2) / (1 + falloff * s2);
}

vec2 CalcUV() {
	return gl_FragCoord.xy / screen_dim;
}