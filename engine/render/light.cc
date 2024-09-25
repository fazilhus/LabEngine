#include "config.h"
#include "light.h"

namespace Render {

	Render::OmniDirLight::OmniDirLight(
		const Math::vec3& pos, const Math::vec4& col, float32 intensity, const Math::vec3& attenuation)
	: pos(pos), color(col), intensity(intensity), attenuation(attenuation) {
	}

	OmniDirLight::OmniDirLight()
		: pos(0.0f), color(1.0f), intensity(1.0f), attenuation({1.0f, 0.0f, 0.0f}) {
	}

} // Render