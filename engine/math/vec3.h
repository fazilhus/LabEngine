#pragma once
#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstdint>

namespace Math {

	class vec3 {
	public:
		float x, y, z;

		vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

		vec3() : vec3(0, 0, 0) {}

		vec3(float v) : vec3(v, v, v) {}

		vec3(const vec3& src) : vec3(src.x, src.y, src.z) {}

		vec3& operator=(const vec3& rhs) {
			this->x = rhs.x;
			this->y = rhs.y;
			this->z = rhs.z;
			return *this;
		}

		const vec3 operator+(const vec3& rhs) const {
			vec3 res{ *this };
			return res += rhs;
		}

		vec3& operator+=(const vec3& rhs) {
			this->x += rhs.x;
			this->y += rhs.y;
			this->z += rhs.z;
			return *this;
		}

		const vec3 operator-() const {
			vec3 res{ *this };
			res.x = -res.x;
			res.y = -res.y;
			res.z = -res.z;
			return res;
		}

		const vec3 operator-(const vec3& rhs) const {
			vec3 res{ *this };
			return res -= rhs;
		}

		vec3& operator-=(const vec3& rhs) {
			this->x -= rhs.x;
			this->y -= rhs.y;
			this->z -= rhs.z;
			return *this;
		}

		const vec3 operator*(const float scalar) const {
			vec3 res{ *this };
			return res *= scalar;
		}

		vec3& operator*=(const float scalar) {
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
			return *this;
		}

		const bool operator==(const vec3& rhs) const {
			return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z);
		}

		const bool operator!=(const vec3& rhs) const {
			return !(*this == rhs);
		}

		float& operator[](const std::size_t i) {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			default: assert(false && "unreachable");
			}
		}

		const float operator[](const std::size_t i) const {
			switch (i) {
			case 0: return this->x;
			case 1: return this->y;
			case 2: return this->z;
			default: assert(false && "unreachable");
			}
		}
	};

	inline float dot(const vec3& a, const vec3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	inline float length(const vec3& a) {
		return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	inline vec3 cross(const vec3& a, const vec3& b) {
		vec3 res{};
		res.x = a.y * b.z - a.z * b.y;
		res.y = a.z * b.x - a.x * b.z;
		res.z = a.x * b.y - a.y * b.x;
		return res;
	}

	inline vec3 normalize(const vec3& a) {
		vec3 res{a};
		float len = length(a);
		return res *= (1 / len);
	}

	inline void vec3print(const vec3& v) {
		printf("%5.2f %5.2f %5.2f\n", v.x, v.y, v.z);
	}

} // Math