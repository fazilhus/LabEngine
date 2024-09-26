#include "config.h"
#include "light.h"

namespace Render {

	Light::Light()
		: ambient(0.2f), diffuse(0.5f), specular(1.0f) {
	}

	Light::Light(const Math::vec3& a, const Math::vec3& d, const Math::vec3& s)
		: ambient(a), diffuse(d), specular(s) {
	}

	DirectionalLight::DirectionalLight()
		: dir({0.0f, -1.0f, 0.0f}), Light() {
	}

	DirectionalLight::DirectionalLight(const Math::vec3& dir, const Math::vec3& a,
		const Math::vec3& d, const Math::vec3& s)
		: dir(dir), Light(a, d, s) {
	}

	PointLight::PointLight()
		: pos(0.0f), Light(), attenuation({1.0f, 0.0f, 0.0f}) {
	}

	PointLight::PointLight(const Math::vec3& pos, const Math::vec3& a,
		const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation)
		: pos(pos), Light(a, d, s), attenuation(attenuation) {
	}

} // Render