#include "config.h"
#include "material.h"

namespace Resource {

	Material::Material()
		: diffuse(), specular(), shininess(32.0f) {
	}

	Material::Material(std::shared_ptr<Resource::Texture> diff, std::shared_ptr<Resource::Texture> spec, float32 shin)
		: diffuse(diff), specular(spec), shininess(shin) {
	}

	void Material::Use(Shader& shader) {
		shader.UploadUniform1i("material.diffuse", 0);
		diffuse->Bind(0);
		shader.UploadUniform1i("material.specular", 1);
		specular->Bind(1);

		shader.UploadUniform1f("material.shininess", shininess);
	}

} // Resource