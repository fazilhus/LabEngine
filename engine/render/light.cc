#include "config.h"
#include "light.h"

#include "math/math.h"

#include <string>

namespace Render {

	Light::Light()
		: ambient(0.1f), diffuse(0.5f), specular(1.0f) {
	}

	Light::Light(const Math::vec3& a, const Math::vec3& d, const Math::vec3& s)
		: ambient(a), diffuse(d), specular(s) {
	}

	DirectionalLight::DirectionalLight()
		: Light(), dir({0.0f, -1.0f, 0.0f}) {
	}

	DirectionalLight::DirectionalLight(const Math::vec3& dir, const Math::vec3& a,
		const Math::vec3& d, const Math::vec3& s)
		: Light(a, d, s), dir(dir) {
	}

	DirectionalLight::DirectionalLight(const DirectionalLight& other)
		: Light(other.GetAmbient(), other.GetDiffuse(), other.GetSpecular()), dir(other.dir) {
	}

	PointLight::PointLight()
		: Light(), pos(0.0f), attenuation({1.0f, 0.0f, 0.0f}) {
	}

	PointLight::PointLight(const Math::vec3& pos, const Math::vec3& a,
		const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation)
		: Light(a, d, s), pos(pos), attenuation(attenuation) {
	}

	PointLight::PointLight(const PointLight& other)
		: Light(other.GetAmbient(), other.GetDiffuse(), other.GetSpecular()),
		pos(other.pos), attenuation(other.attenuation) {
	}

	SpotLight::SpotLight()
		: Light(), pos(0.0f), dir({0.0f, -1.0f, 0.0f}), cutoff(Math::toRad(20.0f)), outerCutoff(Math::toRad(35.0f)),
		attenuation({ 1.0f, 0.0f, 0.0f }) {
	}

	SpotLight::SpotLight(const Math::vec3& pos, const Math::vec3& dir, float32 cutoff, float32 ocutoff,
		const Math::vec3& a, const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation)
		: Light(a, d, s), pos(pos), dir(dir), cutoff(cutoff), outerCutoff(ocutoff), attenuation(attenuation) {
	}

	LightManager::LightManager()
	: globalLight(), pointLights(), pointLightsCount(0), spotLights(), spotLightsCount(0) {
	}

	void LightManager::PushPointLight(const PointLight& pl) {
		if (pointLightsCount >= MAX_NUM_LIGHT_SOURCES) return;

		pointLights.push_back(pl);
		pointLightsCount++;
	}

	void LightManager::PushSpotLight(const SpotLight& sl) {
		if (spotLightsCount >= MAX_NUM_LIGHT_SOURCES) return;

		spotLights.push_back(sl);
		spotLightsCount++;
	}

	void LightManager::SetLightUniforms() {
		for (const auto& shader : lightingShaders) {
			auto s = shader.lock();
			s->Use();

			// set uniforms for the global light source (a.k.a directional light source)
			s->UploadUniform3fv("dlight.dir", globalLight.GetDirection());
			s->UploadUniform3fv("dlight.ambient", globalLight.GetAmbient());
			s->UploadUniform3fv("dlight.diffuse", globalLight.GetDiffuse());
			s->UploadUniform3fv("dlight.specular", globalLight.GetSpecular());

			std::string uniNameLeft = "plights[";
			for (std::size_t i = 0; i < pointLightsCount; ++i) {
				auto temp = uniNameLeft + std::to_string(i);
				s->UploadUniform3fv(temp + "].pos", pointLights[i].GetPos());
				s->UploadUniform3fv(temp + "].ambient", pointLights[i].GetAmbient());
				s->UploadUniform3fv(temp + "].diffuse", pointLights[i].GetDiffuse());
				s->UploadUniform3fv(temp + "].specular", pointLights[i].GetSpecular());
				s->UploadUniform3fv(temp + "].attenuation", pointLights[i].GetAttenuation());
			}
			s->UploadUniform1i("plights_count", pointLightsCount);

			uniNameLeft = "slights[";
			for (std::size_t i = 0; i < spotLightsCount; ++i) {
				auto temp = uniNameLeft + std::to_string(i);
				s->UploadUniform3fv(temp + "].pos", spotLights[i].GetPos());
				s->UploadUniform3fv(temp + "].dir", spotLights[i].GetDirection());
				s->UploadUniform1f(temp + "].cutoff", spotLights[i].GetCutoffAngle());
				s->UploadUniform1f(temp + "].outerCutoff", spotLights[i].GetOuterCutoffAngle());
				s->UploadUniform3fv(temp + "].ambient", spotLights[i].GetAmbient());
				s->UploadUniform3fv(temp + "].diffuse", spotLights[i].GetDiffuse());
				s->UploadUniform3fv(temp + "].specular", spotLights[i].GetSpecular());
				s->UploadUniform3fv(temp + "].attenuation", spotLights[i].GetAttenuation());
			}
			s->UploadUniform1i("slights_count", spotLightsCount);

			s->UnUse();
		}
	}

	void LightManager:: DrawLightSources(const Render::Camera& cam) const {
		auto s = lightSourceShader.lock();
		s->Use();

		s->UploadUniformMat4fv("view", cam.GetView());
		s->UploadUniformMat4fv("perspective", cam.GetPerspective());

		for (std::size_t i = 0; i < pointLightsCount; ++i) {
			auto transform = Math::translate(pointLights[i].GetPos()) * Math::scale(0.1f);
			s->UploadUniformMat4fv("transform", transform);
			s->UploadUniform3fv("light.ambient", pointLights[i].GetAmbient());
			s->UploadUniform3fv("light.diffuse", pointLights[i].GetDiffuse());
			s->UploadUniform3fv("light.specular", pointLights[i].GetSpecular());

			mesh->Draw();
		}

		for (std::size_t i = 0; i < spotLightsCount; ++i) {
			auto transform = Math::translate(spotLights[i].GetPos()) * Math::scale(0.1f);
			s->UploadUniformMat4fv("transform", transform);
			s->UploadUniform3fv("light.ambient", spotLights[i].GetAmbient());
			s->UploadUniform3fv("light.diffuse", spotLights[i].GetDiffuse());
			s->UploadUniform3fv("light.specular", spotLights[i].GetSpecular());

			mesh->Draw();
		}

		s->UnUse();
	}

	void LightManager::DrawLightSourcesForStencil(const Render::Camera& cam, const std::shared_ptr<Resource::Shader>& s) const {
		s->UploadUniformMat4fv("view", cam.GetView());
		s->UploadUniformMat4fv("perspective", cam.GetPerspective());

		for (std::size_t i = 0; i < pointLightsCount; ++i) {
			auto transform = Math::translate(pointLights[i].GetPos()) * Math::scale(pointLights[i].GetRadius());
			s->UploadUniformMat4fv("transform", transform);
			s->UploadUniform3fv("light.ambient", pointLights[i].GetAmbient());
			s->UploadUniform3fv("light.diffuse", pointLights[i].GetDiffuse());
			s->UploadUniform3fv("light.specular", pointLights[i].GetSpecular());

			mesh->Draw();
		}

		//for (std::size_t i = 0; i < spotLightsCount; ++i) {
		//	auto transform = Math::translate(spotLights[i].GetPos()) * Math::scale(0.1f);
		//	s->UploadUniformMat4fv("transform", transform);
		//	s->UploadUniform3fv("light.ambient", spotLights[i].GetAmbient());
		//	s->UploadUniform3fv("light.diffuse", spotLights[i].GetDiffuse());
		//	s->UploadUniform3fv("light.specular", spotLights[i].GetSpecular());
		//
		//	mesh->Draw();
		//}
	}
} // Render