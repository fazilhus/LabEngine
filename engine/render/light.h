#pragma once

#include "config.h"

#include "math/vec4.h"
#include "math/vec3.h"

namespace Render {

	class Light {
	private:
		Math::vec3 ambient;
		Math::vec3 diffuse;
		Math::vec3 specular;

	protected:
		Light();
		Light(const Math::vec3& a, const Math::vec3& d, const Math::vec3& s);

	public:
		inline void SetAmbient(const Math::vec3& a) { this->ambient = a; }
		inline void SetDiffuse(const Math::vec3& d) { this->diffuse = d; }
		inline void SetSpecular(const Math::vec3& s) { this->specular = s; }

		inline const Math::vec3& GetAmbient() const { return ambient; }
		inline Math::vec3& GetAmbient() { return ambient; }
		inline const Math::vec3& GetDiffuse() const { return diffuse; }
		inline Math::vec3& GetDiffuse() { return diffuse; }
		inline const Math::vec3& GetSpecular() const { return specular; }
		inline Math::vec3& GetSpecular() { return specular; }
	};

	class DirectionalLight : public Light {
	private:
		Math::vec3 dir;

	public:
		DirectionalLight();
		DirectionalLight(const Math::vec3& dir, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s);
		~DirectionalLight() = default;

		inline void SetDirection(const Math::vec3& dir) { this->dir = dir; }

		inline const Math::vec3& GetDirection() const { return dir; }
		inline Math::vec3& GetDirection() { return dir; }
	};

	class PointLight : public Light {
	private:
		Math::vec3 pos;

		Math::vec3 attenuation; 

	public:
		PointLight();
		PointLight(const Math::vec3& pos, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation);
		~PointLight() = default;

		inline void SetPos(const Math::vec3& pos) { this->pos = pos; }
		inline void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		inline const Math::vec3& GetPos() const { return pos; }
		inline Math::vec3& GetPos() { return pos; }
		inline const Math::vec3& GetAttenuation() const { return attenuation; }
		inline Math::vec3& GetAttenuation() { return attenuation; }
	};

	class SpotLight : public Light {
	private:
		Math::vec3 pos;
		Math::vec3 dir;
		float32 cutoff;

		Math::vec3 attenuation;

	public:
		SpotLight();
		SpotLight(const Math::vec3& pos, const Math::vec3& dir, float32 cutoff, const Math::vec3& a,
			const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation);
		~SpotLight() = default;

		inline void SetPos(const Math::vec3& pos) { this->pos = pos; }
		inline void SetDirection(const Math::vec3& dir) { this->dir = dir; }
		inline void SetCutoffAngle(float32 angle) { this->cutoff = angle; }
		inline void SetAttenuation(const Math::vec3& attenuation) { this->attenuation = attenuation; }

		inline const Math::vec3& GetPos() const { return pos; }
		inline Math::vec3& GetPos() { return pos; }
		inline const Math::vec3& GetDirection() const { return dir; }
		inline Math::vec3& GetDirection() { return dir; }
		inline float32 GetCutoffAngle() const { return cutoff; }
		inline float32 GetCutoffAngle() { return cutoff; }
		inline const Math::vec3& GetAttenuation() const { return attenuation; }
		inline Math::vec3& GetAttenuation() { return attenuation; }
	};

} // Render