#include "config.h"
#include "material.h"

#include <iostream>

namespace Resource {

	Material::Material()
		: shininess(32.0f) {
	}

	Material::Material(const std::weak_ptr<Resource::Texture>& diff,
		const std::weak_ptr<Resource::Texture>& spec, float32 shin,
		const std::weak_ptr<Shader>& shader)
		: diffuse(diff), specular(spec), shininess(shin), shader(shader) {
	}

	Material::Material(const Material& other)
		: diffuse(other.diffuse), specular(other.specular), shininess(other.shininess),
		shader(other.shader) {
	}

	Material::Material(Material&& other) noexcept
		: diffuse(std::move(other.diffuse)), specular(std::move(other.specular)),
		shininess(std::move(other.shininess)), shader(std::move(other.shader)) {
	}

	Material& Material::operator=(const Material& other) {
		diffuse = other.diffuse;
		specular = other.specular;
		shininess = other.shininess;
		shader = other.shader;
		return *this;
	}

	Material& Material::operator=(Material&& other) noexcept {
		diffuse = std::move(other.diffuse);
		specular = std::move(other.specular);
		shininess = other.shininess;
		shader = std::move(other.shader);
		return *this;
	}

	void Material::Use() {
		shader.lock()->UploadUniform1i("material.diffuse", 0);
		diffuse.lock()->Bind(0);

		shader.lock()->UploadUniform1i("material.specular", 1);
		specular.lock()->Bind(1);

		shader.lock()->UploadUniform1f("material.shininess", shininess);
	}

} // Resource