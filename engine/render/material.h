#pragma once

#include "config.h"
#include "render/texture.h"
#include "render/shader.h"

#include <memory>


namespace Resource {

	class Material {
		std::weak_ptr<Texture> diffuse;
		std::weak_ptr<Texture> specular;
		float32 shininess;

		std::weak_ptr<Shader> shader;

	public:
		Material();
		Material(const std::weak_ptr<Texture>& diff,
		         const std::weak_ptr<Texture>& spec, float32 shin,
		         const std::weak_ptr<Shader>& shader);
		Material(const Material& other);
		Material(Material&& other) noexcept;
		~Material() = default;

		Material& operator=(const Material& other);
		Material& operator=(Material&& other) noexcept;

		void Use();

		void SetShininess(float32 shininess) { this->shininess = shininess; }
		void SetShader(const std::weak_ptr<Shader>& shader) { this->shader = shader; }

		float32 GetShininess() const { return shininess; }
		const std::weak_ptr<Shader>& GetShader() const { return shader; }
	};

} // Resource
