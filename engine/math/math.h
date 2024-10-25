#pragma once

#include <numbers>
#include <random>

namespace Math {

	constexpr float PI = std::numbers::pi_v<float>;
	constexpr float PI_OVER_TWO = PI / 2.0f;

	inline float toRad(float degrees) {
		return (degrees /180.0f) * PI;
	}

	namespace Random {

		static std::random_device rd;
		static std::mt19937 gen(rd());

		static int rand_int(int a, int b) {
			std::uniform_int_distribution<> d(a, b);
			return d(gen);
		}

		static float rand_float() {
			std::uniform_real_distribution<> d(0, 1);
			return d(gen);
		}

		static float rand_float(float a, float b) {
			std::uniform_real_distribution<> d(a, b);
			return d(gen);
		}

	}

} // Math