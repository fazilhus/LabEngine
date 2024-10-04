#include "config.h"
#include "material.h"

#include <iostream>

namespace Resource {

	Material::Material()
		: shininess(32.0f) {
	}

	Material::Material(const std::shared_ptr<Resource::Texture>& diff,
		const std::shared_ptr<Resource::Texture>& spec, float32 shin,
		const std::weak_ptr<Shader>& shader)
		: diffuse(diff), specular(spec), shininess(shin), shader(shader) {
	}

	void Material::Use() {
		shader.lock()->UploadUniform1i("material.diffuse", 0);
		diffuse->Bind(0);

		shader.lock()->UploadUniform1i("material.specular", 1);
		specular->Bind(1);

		shader.lock()->UploadUniform1f("material.shininess", shininess);
	}

	void MaterialManager::NewMaterial(const std::string& name, const std::string& diffTexPath,
		const std::string& specTexPath, float shin, const std::weak_ptr<Shader>& shader) {
		if (materials.contains(name)) {
			std::cerr << "[WARNING] Overwriting existing material " << name << '\n';
			materials[name].reset();
		}

		auto diffTex = std::make_shared<Resource::Texture>(diffTexPath, 1);
		auto specTex = std::make_shared<Resource::Texture>(specTexPath, 1);

		materials[name] = std::make_shared<Resource::Material>(diffTex, specTex, shin, shader);
	}

	std::weak_ptr<Material> MaterialManager::GetMaterial(const std::string& name) const {
		if (!materials.contains(name)) {
			std::cerr << "[ERROR] Trying to access nonexistent material " << name << '\n';
			return {};
		}

		return materials.at(name);
	}

} // Resource