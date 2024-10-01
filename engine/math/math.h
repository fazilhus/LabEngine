#pragma once

#include <numbers>

namespace Math {

	constexpr float PI = std::numbers::pi_v<float>;
	constexpr float PI_OVER_TWO = PI / 2.0f;

	inline float toRad(float degrees) {
		return (degrees /180.0f) * PI;
	}

} // Math