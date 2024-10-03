#include "config.h"
#include "material.h"

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

} // Resource