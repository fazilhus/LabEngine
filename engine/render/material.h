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

		std::weak_ptr<Resource::Shader> shader;

	public:
		Material();
		Material(const std::shared_ptr<Resource::Texture>& diff,
			const std::shared_ptr<Resource::Texture>& spec, float32 shin,
			const std::weak_ptr<Shader>& shader);
		~Material() = default;

		void Use();

		void SetShininess(float32 shininess) { this->shininess = shininess; }
		void SetShader(const std::weak_ptr<Shader>& shader) { this->shader = shader; }

		float32 GetShininess() const { return shininess; }
	};

} // Resource