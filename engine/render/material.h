#pragma once

#include "config.h"
#include "render/texture.h"
#include "render/shader.h"

#include <memory>


namespace Resource {

	class Material {
	protected:
		Math::vec4 ambient = { 1.0f };
		float32 roughness = 1.0f;
		std::weak_ptr<Texture> diffuse;
		std::weak_ptr<Texture> specular;
		float32 shininess = 1.0f;

		std::weak_ptr<Shader> shader;

	public:
		Material();
		Material(const std::weak_ptr<Texture>& diff,
		         const std::weak_ptr<Texture>& spec, float32 shin,
		         const std::weak_ptr<Shader>& s);

		virtual void Use();

		void SetAmbient(const Math::vec4& v) { ambient = v; }
		void SetDiffuseTex(const std::weak_ptr<Texture>& tex) { diffuse = tex; }
		void SetRoughness(float v) { roughness = v; }
		void SetSpecTex(const std::weak_ptr<Texture>& tex) { specular = tex; }
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

		void SetNormTex(const std::weak_ptr<Texture>& tex) { normal = tex; }
	};

} // Resource
