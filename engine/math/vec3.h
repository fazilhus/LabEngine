#pragma once
#include <cmath>
#include <cassert>
#include <cstdio>
#include <cstdint>

/// <summary>
/// 3D vector class
/// </summary>
class vec3 {
public:
	float x, y, z;

	/// <summary>
	/// vec3 constructor
	/// </summary>
	/// <param name="x">float</param>
	/// <param name="y">float</param>
	/// <param name="z">float</param>
	vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

	/// <summary>
	/// zero vec3 constructor
	/// </summary>
	vec3() : vec3(0, 0, 0) {}

	/// <summary>
	/// vec3 copy constructor
	/// </summary>
	/// <param name="src">const vec3&</param>
	vec3(const vec3& src) : vec3(src.x, src.y, src.z) {}

	/// <summary>
	/// vec3 assignment operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>vec3&</returns>
	vec3& operator=(const vec3& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		return *this;
	}

	/// <summary>
	/// vec3 binary plus operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>const vec3</returns>
	const vec3 operator+(const vec3& rhs) const {
		vec3 res{ *this };
		return res += rhs;
	}

	/// <summary>
	/// vec3 plus-assign operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>vec3&</returns>
	vec3& operator+=(const vec3& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		return *this;
	}

	/// <summary>
	/// vec3 unary minus operator
	/// </summary>
	/// <returns>const vec3</returns>
	const vec3 operator-() const {
		vec3 res{ *this };
		res.x = -res.x;
		res.y = -res.y;
		res.z = -res.z;
		return res;
	}

	/// <summary>
	/// vec3 binary minus operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>const vec3</returns>
	const vec3 operator-(const vec3& rhs) const {
		vec3 res{ *this };
		return res -= rhs;
	}

	/// <summary>
	/// vec3 minus-assign operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>vec3&</returns>
	vec3& operator-=(const vec3& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		return *this;
	}

	/// <summary>
	/// vec3 binary mult operator
	/// </summary>
	/// <param name="scalar">float</param>
	/// <returns>const vec3</returns>
	const vec3 operator*(const float scalar) const {
		vec3 res{ *this };
		return res *= scalar;
	}

	/// <summary>
	/// vec3 mult-assign operator
	/// </summary>
	/// <param name="scalar">float</param>
	/// <returns>vec3&</returns>
	vec3& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	/// <summary>
	/// vec3 equal operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>const bool</returns>
	const bool operator==(const vec3& rhs) const {
		return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z);
	}

	/// <summary>
	/// vec3 not-equal operator
	/// </summary>
	/// <param name="rhs">const vec3&</param>
	/// <returns>const bool</returns>
	const bool operator!=(const vec3& rhs) const {
		return !(*this == rhs);
	}

	/// <summary>
	/// vec3 at operator
	/// </summary>
	/// <param name="i">const std::size_t</param>
	/// <returns>float&</returns>
	float& operator[](const std::size_t i) {
		switch (i) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		default: assert(false && "unreachable");
		}
	}

	/// <summary>
	/// vec3 const at operator
	/// </summary>
	/// <param name="i">const std::size_t</param>
	/// <returns>const float</returns>
	const float operator[](const std::size_t i) const {
		switch (i) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		default: assert(false && "unreachable");
		}
	}
};

/// <summary>
/// dot product between two vec3
/// </summary>
/// <param name="a">vec3</param>
/// <param name="b">vec3</param>
/// <returns>float</returns>
float dot(const vec3& a, const vec3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/// <summary>
/// leength of a vec3
/// </summary>
/// <param name="a">vec3</param>
/// <returns>float</returns>
float length(const vec3& a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

/// <summary>
/// cross product between two vec3
/// </summary>
/// <param name="a">vec3</param>
/// <param name="b">vec3</param>
/// <returns>vec3</returns>
vec3 cross(const vec3& a, const vec3& b) {
	vec3 res{};
	res.x = a.y * b.z - a.z * b.y;
	res.y = a.z * b.x - a.x * b.z;
	res.z = a.x * b.y - a.y * b.x;
	return res;
}

/// <summary>
/// vec3 normalization
/// </summary>
/// <param name="a">const vec3&</param>
/// <returns>vec3</returns>
vec3 normalize(const vec3& a) {
	vec3 res{a};
	float len = length(a);
	return res *= (1 / len);
}

/// <summary>
/// print vec3 to stdout
/// </summary>
/// <param name="v">const vec3&</param>
void vec3print(const vec3& v) {
	printf("%5.2f %5.2f %5.2f\n", v.x, v.y, v.z);
}