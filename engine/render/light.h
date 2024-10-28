#pragma once

#include "math/vec3.h"
#include "render/shader.h"
#include "render/mesh.h"
#include "render/camera.h"

#include <vector>
#include <memory>



namespace Render {

	class Light {
		Math::vec3 ambient;
		Math::vec3 diffuse;
		Math::vec3 specular;

	protected:
		Light();
		Light(const Math::vec3& a, const Math::vec3& d, const Math::vec3& s);

	public:
		void SetAmbient(const Math::vec3& a) { this->ambient = a; }
		void SetDiffuse(const Math::vec3& d) { this->diffuse = d; }
		void SetSpecular(const Math::vec3& s) { this->specular = s; }

		const Math::vec3& GetAmbient() const { return ambient; }
		Math::vec3& GetAmbient() { return ambient; }
		const Math::vec3& GetDiffuse() const { return diffuse; }
		Math::vec3& GetDiffuse() { return diffuse; }
		const Math::vec3& GetSpecular() const { return specular; }
		Math::vec3& GetSpecular() { return specular; }
	};

	class DirectionalLight : public Light {
		Math::vec3 dir;

	public:
		DirectionalLight();
		DirectionalLight(const Math::vec3& dir, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s);
		DirectionalLight(const DirectionalLight& other);

		void SetDirection(const Math::vec3& dir) { this->dir = dir; }

		const Math::vec3& GetDirection() const { return dir; }
		Math::vec3& GetDirection() { return dir; }
	};

	class PointLight : public Light {
		Math::vec3 pos;
		float r;
		Math::vec3 attenuation;

		std::shared_ptr<Resource::Mesh> mesh;

	public:
		PointLight();
		PointLight(const Math::vec3& pos, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation);
		PointLight(const PointLight& other);

		void SetPos(const Math::vec3& pos) { this->pos = pos; }
		void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		const Math::vec3& GetPos() const { return pos; }
		Math::vec3& GetPos() { return pos; }
		const Math::vec3& GetAttenuation() const { return attenuation; }
		Math::vec3& GetAttenuation() { return attenuation; }
	};

	class SpotLight : public Light {
		Math::vec3 pos;
		float r;
		Math::vec3 dir;
		float32 cutoff;
		float32 outerCutoff;

		Math::vec3 attenuation;

	public:
		SpotLight();
		SpotLight(const Math::vec3& pos, const Math::vec3& dir, float32 cutoff, float32 ocutoff, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation);

		void SetPos(const Math::vec3& pos) { this->pos = pos; }
		void SetDirection(const Math::vec3& dir) { this->dir = dir; }
		void SetCutoffAngle(float32 angle) { this->cutoff = angle; }
		void SetOuterCutoffAngle(float32 angle) { this->outerCutoff = angle; }
		void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		const Math::vec3& GetPos() const { return pos; }
		Math::vec3& GetPos() { return pos; }
		const Math::vec3& GetDirection() const { return dir; }
		Math::vec3& GetDirection() { return dir; }
		float32 GetCutoffAngle() const { return cutoff; }
		float32 GetCutoffAngle() { return cutoff; }
		float32 GetOuterCutoffAngle() const { return outerCutoff; }
		float32 GetOuterCutoffAngle() { return outerCutoff; }
		const Math::vec3& GetAttenuation() const { return attenuation; }
		Math::vec3& GetAttenuation() { return attenuation; }
	};

	constexpr int MAX_NUM_LIGHT_SOURCES = 64;

	class LightManager {
		DirectionalLight globalLight;
		std::vector<PointLight> pointLights;
		std::size_t pointLightsCount;
		std::vector<SpotLight> spotLights;
		std::size_t spotLightsCount;

		std::vector<std::weak_ptr<Resource::Shader>> lightingShaders;
		std::weak_ptr<Resource::Shader> lightSourceShader;
		std::shared_ptr<Resource::Mesh> mesh;

	public:
		LightManager();

		void PushLightingShader(const std::weak_ptr<Resource::Shader>& s) { this->lightingShaders.push_back(s); }
		void SetLightSourceShader(const std::weak_ptr<Resource::Shader>& s) { this->lightSourceShader = s; }
		void SetMesh(const std::shared_ptr<Resource::Mesh>& m) { this->mesh = m; }

		DirectionalLight& GetGlobalLight() { return globalLight; }
		const DirectionalLight& GetGlobalLight() const { return globalLight; }

		std::vector<PointLight>& GetPointLights() { return pointLights; }
		const std::vector<PointLight>& GetPointLights() const { return pointLights; }
		std::vector<SpotLight>& GetSpotLights() { return spotLights; }
		const std::vector<SpotLight>& GetSpotLights() const { return spotLights; }

		void SetGlobalLight(const DirectionalLight& dl) { globalLight = dl; }
		void PushPointLight(const PointLight& pl);
		void PushSpotLight(const SpotLight& sl);

		void SetLightUniforms();

		void DrawLightSources(const Render::Camera& cam) const;
	};

} // Render