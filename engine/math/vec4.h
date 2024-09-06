#pragma once
#include <cmath>
#include <cstdio>

/// <summary>
/// 4D vector class
/// </summary>
class vec4 {
public:
	float x, y, z, w;

	/// <summary>
	/// vec4 constructor
	/// </summary>
	/// <param name="x">const float</param>
	/// <param name="y">const float</param>
	/// <param name="z">const float</param>
	/// <param name="w">const float</param>
	vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}

	/// <summary>
	/// zero vec4 constructor
	/// </summary>
	vec4() : vec4(0, 0, 0, 0) {}

	/// <summary>
	/// vec4 copy constructor
	/// </summary>
	/// <param name="other">const vec4&</param>
	vec4(const vec4& other) : vec4(other.x, other.y, other.z, other.w) {}

	/// <summary>
	/// vec4 assignment operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>vec4&</returns>
	vec4& operator=(const vec4& rhs) {
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;
		this->w = rhs.w;
		return *this;
	}

	/// <summary>
	/// vec4 binary plus operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const vec4</returns>
	const vec4 operator+(const vec4& rhs) const {
		vec4 res{ *this };
		return res += rhs;
	}

	/// <summary>
	/// vec4 plus-assign operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>vec4&</returns>
	vec4& operator+=(const vec4& rhs) {
		this->x += rhs.x;
		this->y += rhs.y;
		this->z += rhs.z;
		this->w += rhs.w;
		return *this;
	}

	/// <summary>
	/// vec4 unary minus operator
	/// </summary>
	/// <returns>const vec4</returns>
	const vec4 operator-() const {
		vec4 res{ *this };
		res.x = -res.x;
		res.y = -res.y;
		res.z = -res.z;
		res.w = -res.w;
		return res;
	}

	/// <summary>
	/// vec4 binary minus operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const vec4</returns>
	const vec4 operator-(const vec4& rhs) const {
		vec4 res{ *this };
		return res -= rhs;
	}

	/// <summary>
	/// vec4 minus-assign operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>vec4&</returns>
	vec4& operator-=(const vec4& rhs) {
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}

	/// <summary>
	/// vec4 binary mult operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const vec4</returns>
	const vec4 operator*(const float scalar) const {
		vec4 res{ *this };
		return res *= scalar;
	}

	/// <summary>
	/// vec4 mult-assign operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>vec4&</returns>
	vec4& operator*=(const float scalar) {
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		this->w *= scalar;
		return *this;
	}

	/// <summary>
	/// vec4 equal operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const bool</returns>
	const bool operator==(const vec4& rhs) const {
		return (this->x == rhs.x) && (this->y == rhs.y)
			&& (this->z == rhs.z) && (this->w == rhs.w);
	}

	/// <summary>
	/// vec4 not-equal operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const bool</returns>
	const bool operator!=(const vec4& rhs) const {
		return !(*this == rhs);
	}

	/// <summary>
	/// vec4 at operator
	/// </summary>
	/// <param name="i">const uint</param>
	/// <returns>float&</returns>
	float& operator[](const unsigned i) {
		switch (i) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		case 3: return this->w;
		default: assert(false && "unreachable");
		}
	}

	/// <summary>
	/// vec4 const at operator
	/// </summary>
	/// <param name="i">const uint</param>
	/// <returns>const float</returns>
	const float operator[](const unsigned i) const {
		switch (i) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		case 3: return this->w;
		default: assert(false && "unreachable");
		}
	}
};

/// <summary>
/// dot product between two vec4
/// </summary>
/// <param name="a">const vec4&</param>
/// <param name="b">const vec4&</param>
/// <returns>float</returns>
float dot(const vec4& a, const vec4& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/// <summary>
/// length of a vec4
/// </summary>
/// <param name="a">const vec4&</param>
/// <returns>float</returns>
float length(const vec4& a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
}

/// <summary>
/// vec4 normalization
/// </summary>
/// <param name="a">const vec4&</param>
/// <returns>vec4</returns>
vec4 normalize(const vec4& a) {
	vec4 res{ a };
	float len = length(a);
	return res *= (1 / len);
}

/// <summary>
/// print vec4 to stdout
/// </summary>
/// <param name="v">const vec4&</param>
void vec4print(const vec4& v) {
	printf("%5.2f %5.2f %5.2f %5.2f\n", v.x, v.y, v.z, v.w);
}