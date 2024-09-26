#pragma once

#include "config.h"

#include "math/vec4.h"
#include "math/vec3.h"

namespace Render {

	class PointLight {
	private:
		Math::vec3 pos;
		Math::vec3 ambient;
		Math::vec3 diffuse;
		Math::vec3 specular;
		Math::vec3 attenuation; 

	public:
		PointLight(const Math::vec3& pos, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation);
		PointLight();
		~PointLight() = default;

		inline void SetPos(const Math::vec3& pos) { this->pos = pos; }
		inline void SetAmbient(const Math::vec3& a) { this->ambient = a; }
		inline void SetDiffuse(const Math::vec3& d) { this->diffuse = d; }
		inline void SetSpecular(const Math::vec3& s) { this->specular = s; }
		inline void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		inline const Math::vec3& GetPos() const { return pos; }
		inline Math::vec3& GetPos() { return pos; }

		inline const Math::vec3& GetAmbient() const { return ambient; }
		inline Math::vec3& GetAmbient() { return ambient; }
		inline const Math::vec3& GetDiffuse() const { return diffuse; }
		inline Math::vec3& GetDiffuse() { return diffuse; }
		inline const Math::vec3& GetSpecular() const { return specular; }
		inline Math::vec3& GetSpecular() { return specular; }

		inline const Math::vec3& GetAttenuation() const { return attenuation; }
		inline Math::vec3& GetAttenuation() { return attenuation; }
	};

} // Render