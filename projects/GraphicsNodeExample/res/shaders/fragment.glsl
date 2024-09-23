#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;
uniform vec3 light_pos;
uniform vec4 light_col;
uniform float light_int;
uniform vec3 cam_pos;
uniform sampler2D tex; 
out vec4 oColor;
void main()
{
	vec4 ambient_col = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 surf_col = texture(tex, iUV);
	vec4 ambient_term = ambient_col * light_col;

	vec3 norm = normalize(iNorm);
	vec3 ldir = normalize(light_pos - iPos);
	float diff = max(dot(norm, ldir), 0.0);
	vec4 diffuse_term = light_col * diff;

	vec3 camdir = normalize(cam_pos - iPos);
	vec3 halfwaydir = normalize(ldir + camdir);
	float spec = pow(max(dot(norm, halfwaydir), 0.0), light_int);
	vec4 spec_term = spec * light_col;

	oColor = (ambient_term + diffuse_term + spec_term) * surf_col;
}
