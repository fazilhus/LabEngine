#pragma once

constexpr float PI = 3.14159265359f;
constexpr float PI_OVER_TWO = 1.57079632679f;

namespace Math {

	inline float toRad(float degrees) {
		return (degrees /180.0f) * PI;
	}

} // Math