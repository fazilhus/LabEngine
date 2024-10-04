#pragma once

#include "config.h"
#include "render/texture.h"
#include "render/shader.h"

#include <unordered_map>
#include <memory>


namespace Resource {
	class Material {
		std::shared_ptr<Texture> diffuse;
		std::shared_ptr<Texture> specular;
		float32 shininess;

		std::weak_ptr<Shader> shader;

	public:
		Material();
		Material(const std::shared_ptr<Texture>& diff,
		         const std::shared_ptr<Texture>& spec, float32 shin,
		         const std::weak_ptr<Shader>& shader);
		~Material() = default;

		void Use();

		void SetShininess(float32 shininess) { this->shininess = shininess; }
		void SetShader(const std::weak_ptr<Shader>& shader) { this->shader = shader; }

		float32 GetShininess() const { return shininess; }
	};

	class MaterialManager {
		std::unordered_map<std::string, std::shared_ptr<Material>> materials;

	public:
		MaterialManager() = default;
		MaterialManager(const MaterialManager&) = delete;
		MaterialManager(MaterialManager&&) = delete;
		~MaterialManager() = default;

		MaterialManager& operator=(const MaterialManager&) = delete;
		MaterialManager& operator=(MaterialManager&&) = delete;

		void NewMaterial(const std::string& name, const std::string& diffTexPath,
			const std::string& specTexPath, float shin, const std::weak_ptr<Shader>& shader);
		std::weak_ptr<Material> GetMaterial(const std::string& name) const;
	};
} // Resource
