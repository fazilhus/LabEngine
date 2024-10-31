#pragma once
#include <cassert>
#include <cstdio>

namespace Math {

	class vec2 {
	public:
		float x, y;

		vec2(float x, float y) : x(x), y(y) {}

		vec2() : vec2(0, 0) {}

		vec2(float v) : vec2(v, v) {}

		vec2(const vec2& src) : vec2(src.x, src.y) {}

		vec2& operator=(const vec2& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			return *this;
		}

		vec2 operator+(const vec2& rhs) const {
			vec2 res{ *this };
			return res += rhs;
		}

		vec2& operator+=(const vec2& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			return *this;
		}

		vec2 operator-() const {
			vec2 res{ *this };
			res.x = -res.x;
			res.y = -res.y;
			return res;
		}

		vec2 operator-(const vec2& rhs) const {
			vec2 res{ *this };
			return res -= rhs;
		}

		vec2& operator-=(const vec2& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			return *this;
		}

		vec2 operator*(float scalar) const {
			vec2 res{ *this };
			return res *= scalar;
		}

		vec2& operator*=(float scalar) {
			this->x *= scalar;
			this->y *= scalar;
			return *this;
		}

		vec2 operator*(const vec2& rhs) const {
			vec2 res{ *this };
			return res *= rhs;
		}

		vec2& operator*=(const vec2& rhs) {
			this->x *= rhs.x;
			this->y *= rhs.y;
			return *this;
		}

		bool operator==(const vec2& rhs) const {
			return (this->x == rhs.x) && (this->y == rhs.y);
		}

		bool operator!=(const vec2& rhs) const {
			return !(*this == rhs);
		}

		float& operator[](std::size_t i) {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			default: assert(false && "unreachable");
			}
		}

		float operator[](std::size_t i) const {
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

} // Math

inline void vec2print(const Math::vec2& v) {
	printf("%5.2f %5.2f\n", v.x, v.y);
}