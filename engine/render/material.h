#pragma once

#include "config.h"
#include "render/texture.h"
#include "render/shader.h"

#include <memory>


namespace Resource {

	class Material {
	private:
		std::shared_ptr<Resource::Texture> diffuse;
		std::shared_ptr<Resource::Texture> specular;
		float32 shininess;

	public:
		Material();
		Material(std::shared_ptr<Resource::Texture> diff, std::shared_ptr<Resource::Texture> spec, float32 shin);
		~Material() = default;

		void Use(Shader& shader);

		inline void SetShininess(float32 shininess) { this->shininess = shininess; }

		inline float32 GetShininess() const { return shininess; }
	};

} // Resource