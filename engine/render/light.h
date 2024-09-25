#pragma once

#include "config.h"

#include "math/vec4.h"
#include "math/vec3.h"

namespace Render {

	class OmniDirLight {
	private:
		Math::vec3 pos;
		Math::vec4 color;
		Math::vec3 attenuation; 
		float32 intensity;

	public:
		OmniDirLight(const Math::vec3& pos, const Math::vec4& col, float32 intensity, const Math::vec3& attenuation);
		OmniDirLight();
		~OmniDirLight() = default;

		inline void SetPos(const Math::vec3& pos) { this->pos = pos; }
		inline void SetColor(const Math::vec4& col) { this->color = col; }
		inline void SetIntensity(float32 intensity) { this->intensity = intensity; }
		inline void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		inline const Math::vec3& GetPos() const { return pos; }
		inline Math::vec3& GetPos() { return pos; }

		inline const Math::vec4& GetColor() const { return color; }
		inline Math::vec4& GetColor() { return color; }

		inline float32 GetIntensity() const { return intensity; }

		inline const Math::vec3& GetAttenuation() const { return attenuation; }
		inline Math::vec3& GetAttenuation() { return attenuation; }
	};

} // Render