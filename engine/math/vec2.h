#pragma once
#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstdint>

namespace Math {

	class vec2 {
	public:
		float x, y;

		vec2(const float x, const float y) : x(x), y(y) {}

		vec2() : vec2(0, 0) {}

		vec2(const vec2& src) : vec2(src.x, src.y) {}

		vec2& operator=(const vec2& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			return *this;
		}

		const vec2 operator+(const vec2& rhs) const {
			vec2 res{ *this };
			return res += rhs;
		}

		vec2& operator+=(const vec2& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		const vec2 operator-() const {
			vec2 res{ *this };
			res.x = -res.x;
			res.y = -res.y;
			return res;
		}

		const vec2 operator-(const vec2& rhs) const {
			vec2 res{ *this };
			return res -= rhs;
		}

		vec2& operator-=(const vec2& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		const vec2 operator*(const float scalar) const {
			vec2 res{ *this };
			return res *= scalar;
		}

		vec2& operator*=(const float scalar) {
			this->x *= scalar;
			this->y *= scalar;
			return *this;
		}

		const bool operator==(const vec2& rhs) const {
			return (this->x == rhs.x) && (this->y == rhs.y);
		}

		const bool operator!=(const vec2& rhs) const {
			return !(*this == rhs);
		}

		float& operator[](const std::size_t i) {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			default: assert(false && "unreachable");
			}
		}

		const float operator[](const std::size_t i) const {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			default: assert(false && "unreachable");
			}
		}
	};

	inline float dot(const vec2& a, const vec2& b) {
		return a.x * b.x + a.y * b.y;
	}

	inline float length(const vec2& a) {
		return sqrtf(a.x * a.x + a.y * a.y);
	}

	inline vec2 normalize(const vec2& a) {
		vec2 res{a};
		float len = length(a);
		return res *= (1 / len);
	}

	inline void vec2print(const vec2& v) {
		printf("%5.2f %5.2f\n", v.x, v.y);
	}

} // Math