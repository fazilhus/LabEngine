#version 460 core
uniform sampler2D gPos;
uniform sampler2D gCol;
uniform sampler2D gNorm;

uniform vec2 screen_dim;
uniform vec3 cam_pos;

struct DirectionalLight {
	vec3 dir;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirectionalLight light;

layout(location=0) out vec4 oColor;

vec3 CalcDirectionalLight(vec3 norm, vec3 cam_dir, vec3 diff, float spec);

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

	vec3 tempCol = CalcDirectionalLight(norm, cam_dir, diff, spec);

	float gamma = 0.9;
	tempCol = pow(tempCol, vec3(1.0 / gamma));
	oColor = vec4(tempCol, 1.0);
}

vec3 CalcDirectionalLight(vec3 norm, vec3 cam_dir, vec3 diff, float spec)
{
	vec3 ambient = light.ambient * diff;

	vec3 ldir = normalize(-light.dir);
	float d = max(dot(norm, ldir), 0.0);
	vec3 diffuse = light.diffuse * d * diff;

	vec3 halfwaydir = normalize(ldir + cam_dir);
	float s = pow(max(dot(norm, -halfwaydir), d), spec * 128.0);
	vec3 specular = light.specular * s * diff;

	return ambient + diffuse + specular;
}

vec2 CalcUV() {
	return gl_FragCoord.xy / screen_dim;
}