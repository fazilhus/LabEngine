#pragma once

#include "config.h"
#include "render/texture.h"
#include "render/shader.h"

#include <memory>


namespace Resource {

	class Material {
	protected:
		std::weak_ptr<Texture> diffuse;
		std::weak_ptr<Texture> specular;
		float32 shininess;

		std::weak_ptr<Shader> shader;

	public:
		Material();
		Material(const std::weak_ptr<Texture>& diff,
		         const std::weak_ptr<Texture>& spec, float32 shin,
		         const std::weak_ptr<Shader>& s);

		virtual void Use();

		void SetShininess(float32 shin) { this->shininess = shin; }
		void SetShader(const std::weak_ptr<Shader>& s) { this->shader = s; }

		float32 GetShininess() const { return shininess; }
		const std::weak_ptr<Shader>& GetShader() const { return shader; }
	};

	class NormalMapMaterial : public Material {
		std::weak_ptr<Texture> normal;

	public:
		NormalMapMaterial() = default;
		NormalMapMaterial(const std::weak_ptr<Texture>& diff, const std::weak_ptr<Texture>& spec,
			const std::weak_ptr<Texture>& norm, float32 shin, const std::weak_ptr<Shader>& s);

		void Use() override;
	};

} // Resource
