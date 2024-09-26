#pragma once

#include "config.h"
#include "math/vec3.h"

namespace Resource {

	class Material {
	private:
		Math::vec3 ambient;
		Math::vec3 diffuse;
		Math::vec3 specular;
		float32 shininess;

	public:
		Material();
		Material(Math::vec3 amb, Math::vec3 diff, Math::vec3 spec, float32 shin);
		~Material() = default;

		inline void SetAmbient(Math::vec3 ambient) { this->ambient = ambient; }
		inline void SetDiffuse(Math::vec3 diffuse) { this->diffuse = diffuse; }
		inline void SetSpecular(Math::vec3 specular) { this->specular = specular; }
		inline void SetShininess(float32 shininess) { this->shininess = shininess; }

		inline Math::vec3 GetAmbient() const { return ambient; }
		inline Math::vec3 GetDiffuse() const { return diffuse; }
		inline Math::vec3 GetSpecular() const { return specular; }
		inline float32 GetShininess() const { return shininess; }
	};

} // Resource