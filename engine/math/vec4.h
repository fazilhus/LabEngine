#pragma once
#include <cstdio>

namespace Math {

	class vec4 {
	public:
		float x, y, z, w;

		vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

		vec4() : vec4(0, 0, 0, 0) {}

		vec4(float v) : vec4(v, v, v, v) {}

		vec4(const vec4& other) : vec4(other.x, other.y, other.z, other.w) {}

		vec4& operator=(const vec4& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			this->w = rhs.w;
			return *this;
		}

		const vec4 operator+(const vec4& rhs) const {
			vec4 res{ *this };
			return res += rhs;
		}

		vec4& operator+=(const vec4& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			this->w += rhs.w;
			return *this;
		}

		const vec4 operator-() const {
			vec4 res{ *this };
			res.x = -res.x;
			res.y = -res.y;
			res.z = -res.z;
			res.w = -res.w;
			return res;
		}

		const vec4 operator-(const vec4& rhs) const {
			vec4 res{ *this };
			return res -= rhs;
		}

		vec4& operator-=(const vec4& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			this->w -= rhs.w;
			return *this;
		}

		const vec4 operator*(const float scalar) const {
			vec4 res{ *this };
			return res *= scalar;
		}

		vec4& operator*=(const float scalar) {
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
			this->w *= scalar;
			return *this;
		}

		const bool operator==(const vec4& rhs) const {
			return (this->x == rhs.x) && (this->y == rhs.y)
				&& (this->z == rhs.z) && (this->w == rhs.w);
		}

		const bool operator!=(const vec4& rhs) const {
			return !(*this == rhs);
		}

		float& operator[](const std::size_t i) {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default: assert(false && "unreachable");
			}
		}

		const float operator[](const std::size_t i) const {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			case 3: return this->w;
			default: assert(false && "unreachable");
			}
		}
	};

	inline float dot(const vec4& a, const vec4& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}

	inline float length(const vec4& a) {
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	}

	inline vec4 normalize(const vec4& a) {
		vec4 res{ a };
		float len = length(a);
		return res *= (1 / len);
	}

} // Math

inline void vec4print(const Math::vec4& v) {
	printf("%5.2f %5.2f %5.2f %5.2f\n", v.x, v.y, v.z, v.w);
}