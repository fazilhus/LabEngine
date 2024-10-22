#include "config.h"
#include "material.h"

namespace Resource {

	Material::Material()
		: shininess(8.0f) {
	}

	Material::Material(const std::weak_ptr<Resource::Texture>& diff,
		const std::weak_ptr<Resource::Texture>& spec, float32 shin,
		const std::weak_ptr<Shader>& s)
		: diffuse(diff), specular(spec), shininess(shin), shader(s) {
	}

	void Material::Use() {
		const auto& s = shader.lock();

		s->UploadUniform1i("material.diffuse", 0);
		diffuse.lock()->Bind(0);

		s->UploadUniform1i("material.specular", 1);
		specular.lock()->Bind(1);

		s->UploadUniform4fv("material.ambient", ambient);
		s->UploadUniform1f("material.roughness", roughness);
		s->UploadUniform1f("material.shininess", shininess);
	}

	NormalMapMaterial::NormalMapMaterial(const std::weak_ptr<Texture>& diff, const std::weak_ptr<Texture>& spec,
		const std::weak_ptr<Texture>& norm, float32 shin, const std::weak_ptr<Shader>& s)
		: Material(diff, spec, shin, s), normal(norm) {
	}

	void NormalMapMaterial::Use() {
		const auto& s = shader.lock();

		s->UploadUniform1i("material.diffuse", 0);
		diffuse.lock()->Bind(0);

		s->UploadUniform1i("material.specular", 1);
		specular.lock()->Bind(1);

		s->UploadUniform1i("material.normal", 2);
		normal.lock()->Bind(2);

		s->UploadUniform4fv("material.ambient", ambient);
		s->UploadUniform1f("material.roughness", roughness);
		s->UploadUniform1f("material.shininess", shininess);
	}
} // Resource