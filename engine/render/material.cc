#include "config.h"
#include "material.h"

namespace Resource {

	Material::Material()
		: ambient(Math::vec3(0.1f)), diffuse(Math::vec3(0.8f)), specular(Math::vec3(0.9f)), shininess(32.0f) {
	}

	Material::Material(Math::vec3 amb, Math::vec3 diff, Math::vec3 spec, float32 shin)
		: ambient(amb), diffuse(diff), specular(spec), shininess(shin) {
	}

} // Resource