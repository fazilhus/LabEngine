#include "config.h"
#include "light.h"

namespace Render {

	PointLight::PointLight(
		const Math::vec3& pos, const Math::vec3& a,
		const Math::vec3& d, const Math::vec3& s, const Math::vec3& attenuation)
	: pos(pos), ambient(a), diffuse(d), specular(s), attenuation(attenuation) {
	}

	PointLight::PointLight()
		: pos(0.0f), ambient(0.2f), diffuse(0.5f), specular(1.0f), attenuation({1.0f, 0.0f, 0.0f}) {
	}

} // Render