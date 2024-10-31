#version 460 core
layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNorm;
layout(location=2) in vec2 iUV;
layout(location=3) in mat3 iTBN;

layout(location=0) out vec3 gPos;
layout(location=1) out vec4 gCol;
layout(location=2) out vec3 gNorm;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D normal;
	vec4 ambient;
	float roughness;
	float shininess;
};

uniform Material material;

void main()
{
	vec4 col = texture(material.diffuse, iUV);
	if (col.a < 0.5)
		discard;

	gPos = iPos;
	gCol.rgb = texture(material.diffuse, iUV).rgb;
	gCol.a = texture(material.specular, iUV).g;
	gNorm = normalize(iTBN * (texture(material.normal, iUV).rgb * 2.0 - 1.0));
}