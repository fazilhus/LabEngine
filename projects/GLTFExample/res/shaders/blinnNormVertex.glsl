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
	mat4 model = perspective * view * transform;
	gl_Position = model * vec4(iPos, 1);

	vec3 T = normalize(vec3(model * iTan));
	vec3 N = normalize(vec3(model * vec4(iNorm, 0.0)));
	vec3 B = cross(N, T);
	oTBN = mat3(T, B, N);

	oPos = (transform * vec4(iPos, 1.0)).xyz;
	oNorm = mat3(transpose(inverse(transform))) * iNorm;
	oUV = iUV;
}
