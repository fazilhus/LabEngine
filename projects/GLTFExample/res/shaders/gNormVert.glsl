#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;
layout(location=3) in vec4 iTan;

layout(location=0) out vec3 oPos;
layout(location=1) out vec3 oNorm;
layout(location=2) out vec2 oUV;
layout(location=3) out mat3 oTBN;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 perspective;

void main()
{
	gl_Position = perspective * view * transform * vec4(iPos, 1);
	vec3 norm = mat3(transpose(inverse(transform))) * iNorm;
	vec3 T = normalize(vec3(transform * vec4(iTan.rgb, 0.0)));
	vec3 N = normalize(vec3(transform * vec4(norm, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T) * iTan.w;
	oTBN = mat3(T, B, N);

	oPos = (transform * vec4(iPos, 1.0)).xyz;
	oNorm = norm;
	oUV = iUV;
}