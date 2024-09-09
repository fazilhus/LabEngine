#pragma once

#include <cmath>
#include <cstdio>
#include <cassert>

#include "vec3.h"
#include "vec4.h"

/// <summary>
/// 4x4 matrix class
/// </summary>
class mat4;

/// <summary>
/// mat4 transposition
/// </summary>
/// <param name="m">const mat4&</param>
/// <returns>mat4</returns>
mat4 transpose(const mat4& m);

class mat4 {
public:
	vec4 m[4];

	/// <summary>
	/// identity mat4 constructor
	/// </summary>
	mat4() {
		m[0] = vec4{ 1, 0, 0, 0 };
		m[1] = vec4{ 0, 1, 0, 0 };
		m[2] = vec4{ 0, 0, 1, 0 };
		m[3] = vec4{ 0, 0, 0, 1 };
	}

	/// <summary>
	/// mat4 constructor
	/// </summary>
	/// <param name="r0">const vec4&</param>
	/// <param name="r1">const vec4&</param>
	/// <param name="r2">const vec4&</param>
	/// <param name="r3">const vec4&</param>
	mat4(const vec4& r0, const vec4& r1, const vec4& r2, const vec4& r3) {
		m[0] = r0; m[1] = r1; m[2] = r2; m[3] = r3;
	}

	/// <summary>
	/// mat4 copy constructor
	/// </summary>
	/// <param name="other">const mat4&</param>
	mat4(const mat4& other) {
		for (std::size_t i = 0; i < 4; ++i) {
			m[i] = other[i];
		}
	}

	/// <summary>
	/// static method that returns zero mat4
	/// </summary>
	/// <returns>mat4</returns>
	static mat4 zero() {
		return mat4{
			vec4{},
			vec4{},
			vec4{},
			vec4{}
		};
	}

	/// <summary>
	/// static method that returns identity mat4
	/// </summary>
	/// <returns>mat4</returns>
	static mat4 identity() {
		return mat4{
			vec4{ 1, 0, 0, 0 },
			vec4{ 0, 1, 0, 0 },
			vec4{ 0, 0, 1, 0 },
			vec4{ 0, 0, 0, 1 }
		};
	}

	/// <summary>
	/// mat4 assign operator
	/// </summary>
	/// <param name="other">const mat4&</param>
	/// <returns>mat4&</returns>
	mat4& operator=(const mat4& other) {
		for (std::size_t i = 0; i < 4; ++i) {
			m[i] = other[i];
		}
		return *this;
	}

	/// <summary>
	/// mat4 binary plus operator
	/// </summary>
	/// <param name="rhs">const mat4&</param>
	/// <returns>const mat4</returns>
	const mat4 operator+(const mat4& rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] + rhs[i][j];
			}
		}
		return res;
	}

	/// <summary>
	/// mat4 binary minus operator
	/// </summary>
	/// <param name="rhs">const mat4&</param>
	/// <returns>const mat4</returns>
	const mat4 operator-(const mat4& rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] - rhs[i][j];
			}
		}
		return res;
	}

	/// <summary>
	/// mat4 binary mult operator
	/// </summary>
	/// <param name="rhs">const mat4&</param>
	/// <returns>const mat4</returns>
	const mat4 operator*(const mat4& rhs) const {
		mat4 res = mat4::zero();
		mat4 thisT = transpose(*this);
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = dot(thisT[j], rhs[i]);
			}
		}
		return res;
	}

	/// <summary>
	/// mat4 binary mult operator (scale)
	/// </summary>
	/// <param name="rhs">const float</param>
	/// <returns>const mat4</returns>
	const mat4 operator*(const float rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] * rhs;
			}
		}
		return res;
	}

	/// <summary>
	/// mat4 binary mult operator
	/// </summary>
	/// <param name="rhs">const vec4&</param>
	/// <returns>const mat4</returns>
	const vec4 operator*(const vec4& rhs) const {
		vec4 res{};
		mat4 thisT = transpose(*this);
		for (std::size_t i = 0; i < 4; ++i) {
			res[i] = dot(thisT[i], rhs);
		}
		return res;
	}

	/// <summary>
	/// mat4 equal operator
	/// </summary>
	/// <param name="rhs">const mat4&</param>
	/// <returns>const bool</returns>
	const bool operator==(const mat4& rhs) const {
		for (std::size_t i = 0; i < 4; ++i) {
			if (this->m[i] != rhs[i]) return false;
		}
		return true;
	}

	/// <summary>
	/// mat4 not-equal operator
	/// </summary>
	/// <param name="rhs">const mat4&</param>
	/// <returns>const bool</returns>
	const bool operator!=(const mat4& rhs) const {
		return !(*this == rhs);
	}

	/// <summary>
	/// mat4 at operator
	/// </summary>
	/// <param name="i">const std::size_t</param>
	/// <returns>vec4&</returns>
	vec4& operator[](const std::size_t i) {
		return this->m[i];
	}

	/// <summary>
	/// mat4 const at operator
	/// </summary>
	/// <param name="i">const std::size_t</param>
	/// <returns>const vec4&</returns>
	const vec4& operator[](const std::size_t i) const {
		return this->m[i];
	}
};

/// <summary>
/// print mat4 to stdout
/// </summary>
/// <param name="m">const mat4&</param>
inline void mat4print(const mat4& m) {
	for (std::size_t i = 0; i < 4; ++i) {
		vec4print(m[i]);
	}
	printf("\n");
}

/// <summary>
/// determinant of a mat4
/// </summary>
/// <param name="m">const mat4&</param>
/// <returns>float</returns>
inline float determinant(const mat4& m) {
	// calculation of needed 2x2 and 3x3 determinants
	float det2233 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
	float det2133 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
	float det2132 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
	float det112233 = m[1][1] * det2233 - m[1][2] * det2133 + m[1][3] * det2132;

	float det2033 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
	float det2032 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
	float det102233 = m[1][0] * det2233 - m[1][2] * det2033 + m[1][3] * det2032;

	float det2031 = m[2][0] * m[3][1] - m[2][1] * m[3][0];
	float det102133 = m[1][0] * det2133 - m[1][1] * det2033 + m[1][3] * det2031;

	float det102132 = m[1][0] * det2132 - m[1][1] * det2032 + m[1][2] * det2031;
	float det = m[0][0] * det112233 - m[0][1] * det102233 + m[0][2] * det102132 - m[0][3] * det102132;
	return det;
}

/// <summary>
/// inverse of a mat4
/// </summary>
/// <param name="m">const mat4&</param>
/// <returns>mat4</returns>
inline mat4 inverse(const mat4& m) {
	mat4 adj = mat4::zero();

	// calculation of an adjugative
	adj[0][0] =
		+ m[1][1] * m[2][2] * m[3][3]
		- m[1][1] * m[2][3] * m[3][2]
		- m[2][1] * m[1][2] * m[3][3]
		+ m[2][1] * m[1][3] * m[3][2]
		+ m[3][1] * m[1][2] * m[2][3]
		- m[3][1] * m[1][3] * m[2][2];

	adj[0][1] =
		- m[0][1] * m[2][2] * m[3][3]
		+ m[0][1] * m[2][3] * m[3][2]
		+ m[2][1] * m[0][2] * m[3][3]
		- m[2][1] * m[0][3] * m[3][2]
		- m[3][1] * m[0][2] * m[2][3]
		+ m[3][1] * m[0][3] * m[2][2];

	adj[0][2] =
		+ m[0][1] * m[1][2] * m[3][3]
		- m[0][1] * m[1][3] * m[3][2]
		- m[1][1] * m[0][2] * m[3][3]
		+ m[1][1] * m[0][3] * m[3][2]
		+ m[3][1] * m[0][2] * m[1][3]
		- m[3][1] * m[0][3] * m[1][2];

	adj[0][3] =
		- m[0][1] * m[1][2] * m[2][3]
		+ m[0][1] * m[1][3] * m[2][2]
		+ m[1][1] * m[0][2] * m[2][3]
		- m[1][1] * m[0][3] * m[2][2]
		- m[2][1] * m[0][2] * m[1][3]
		+ m[2][1] * m[0][3] * m[1][2];

	adj[1][0] =
		- m[1][0] * m[2][2] * m[3][3]
		+ m[1][0] * m[2][3] * m[3][2]
		+ m[2][0] * m[1][2] * m[3][3]
		- m[2][0] * m[1][3] * m[3][2]
		- m[3][0] * m[1][2] * m[2][3]
		+ m[3][0] * m[1][3] * m[2][2];

	adj[1][1] =
		+ m[0][0] * m[2][2] * m[3][3]
		- m[0][0] * m[2][3] * m[3][2]
		- m[2][0] * m[0][2] * m[3][3]
		+ m[2][0] * m[0][3] * m[3][2]
		+ m[3][0] * m[0][2] * m[2][3]
		- m[3][0] * m[0][3] * m[2][2];

	adj[1][2] =
		- m[0][0] * m[1][2] * m[3][3]
		+ m[0][0] * m[1][3] * m[3][2]
		+ m[1][0] * m[0][2] * m[3][3]
		- m[1][0] * m[0][3] * m[3][2]
		- m[3][0] * m[0][2] * m[1][3]
		+ m[3][0] * m[0][3] * m[1][2];

	adj[1][3] =
		+ m[0][0] * m[1][2] * m[2][3]
		- m[0][0] * m[1][3] * m[2][2]
		- m[1][0] * m[0][2] * m[2][3]
		+ m[1][0] * m[0][3] * m[2][2]
		+ m[2][0] * m[0][2] * m[1][3]
		- m[2][0] * m[0][3] * m[1][2];

	adj[2][0] =
		+ m[1][0] * m[2][1] * m[3][3]
		- m[1][0] * m[2][3] * m[3][1]
		- m[2][0] * m[1][1] * m[3][3]
		+ m[2][0] * m[1][3] * m[3][1]
		+ m[3][0] * m[1][1] * m[2][3]
		- m[3][0] * m[1][3] * m[2][1];

	adj[2][1] =
		- m[0][0] * m[2][1] * m[3][3]
		+ m[0][0] * m[2][3] * m[3][1]
		+ m[2][0] * m[0][1] * m[3][3]
		- m[2][0] * m[0][3] * m[3][1]
		- m[3][0] * m[0][1] * m[2][3]
		+ m[3][0] * m[0][3] * m[2][1];

	adj[2][2] =
		+ m[0][0] * m[1][1] * m[3][3]
		- m[0][0] * m[1][3] * m[3][1]
		- m[1][0] * m[0][1] * m[3][3]
		+ m[1][0] * m[0][3] * m[3][1]
		+ m[3][0] * m[0][1] * m[1][3]
		- m[3][0] * m[0][3] * m[1][1];

	adj[2][3] =
		- m[0][0] * m[1][1] * m[2][3]
		+ m[0][0] * m[1][3] * m[2][1]
		+ m[1][0] * m[0][1] * m[2][3]
		- m[1][0] * m[0][3] * m[2][1]
		- m[2][0] * m[0][1] * m[1][3]
		+ m[2][0] * m[0][3] * m[1][1];

	adj[3][0] =
		- m[1][0] * m[2][1] * m[3][2]
		+ m[1][0] * m[2][2] * m[3][1]
		+ m[2][0] * m[1][1] * m[3][2]
		- m[2][0] * m[1][2] * m[3][1]
		- m[3][0] * m[1][1] * m[2][2]
		+ m[3][0] * m[1][2] * m[2][1];

	adj[3][1] =
		+ m[0][0] * m[2][1] * m[3][2]
		- m[0][0] * m[2][2] * m[3][1]
		- m[2][0] * m[0][1] * m[3][2]
		+ m[2][0] * m[0][2] * m[3][1]
		+ m[3][0] * m[0][1] * m[2][2]
		- m[3][0] * m[0][2] * m[2][1];

	adj[3][2] =
		- m[0][0] * m[1][1] * m[3][2]
		+ m[0][0] * m[1][2] * m[3][1]
		+ m[1][0] * m[0][1] * m[3][2]
		- m[1][0] * m[0][2] * m[3][1]
		- m[3][0] * m[0][1] * m[1][2]
		+ m[3][0] * m[0][2] * m[1][1];

	adj[3][3] =
		+ m[0][0] * m[1][1] * m[2][2]
		- m[0][0] * m[1][2] * m[2][1]
		- m[1][0] * m[0][1] * m[2][2]
		+ m[1][0] * m[0][2] * m[2][1]
		+ m[2][0] * m[0][1] * m[1][2]
		- m[2][0] * m[0][2] * m[1][1];

	float det = +m[0][0] * adj[0][0] + m[0][1] * adj[1][0] + m[0][2] * adj[2][0] + m[0][3] * adj[3][0];
	det = 1 / det;

	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			adj[i][j] *= det;
		}
	}

	return adj;
}

/// <summary>
/// transposition of a mat4
/// </summary>
/// <param name="m">const mat4&</param>
/// <returns>mat4</returns>
inline mat4 transpose(const mat4& m) {
	mat4 res = mat4::zero();
	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			res[i][j] = m[j][i];
		}
	}
	return res;
}

/// <summary>
/// mat4 rotation matrix around x-axis
/// </summary>
/// <param name="rad">float</param>
/// <returns>mat4</returns>
inline mat4 rotationx(const float rad) {
	mat4 res = mat4::zero();
	float cosine = cosf(rad);
	float sine = sinf(rad);
	res[0] = vec4{ 1, 0, 0, 0 };
	res[1] = vec4{ 0, cosine, sine, 0 };
	res[2] = vec4{ 0, -sine, cosine, 0 };
	res[3] = vec4{ 0, 0, 0, 1 };
	return res;
}

/// <summary>
/// mat4 rotation matrix around y-axis
/// </summary>
/// <param name="rad">float</param>
/// <returns>mat4</returns>
inline mat4 rotationy(const float rad) {
	mat4 res = mat4::zero();
	float cosine = cosf(rad);
	float sine = sinf(rad);
	res[0] = vec4{ cosine, 0, -sine, 0 };
	res[1] = vec4{ 0, 1, 0, 0 };
	res[2] = vec4{ sine, 0, cosine, 0 };
	res[3] = vec4{ 0, 0, 0, 1 };
	return res;
}

/// <summary>
/// mat4 rotation matrix around z-axis
/// </summary>
/// <param name="rad">float</param>
/// <returns>mat4</returns>
inline mat4 rotationz(const float rad) {
	mat4 res = mat4::zero();
	float cosine = cosf(rad);
	float sine = sinf(rad);
	res[0] = vec4{ cosine, sine, 0, 0 };
	res[1] = vec4{ -sine, cosine, 0, 0 };
	res[2] = vec4{ 0, 0, 1, 0 };
	res[3] = vec4{ 0, 0, 0, 1 };
	return res;
}

/// <summary>
/// mat4 rotation matrix around arbitrary axis
/// </summary>
/// <param name="v">const vec3&</param>
/// <param name="rad">const float</param>
/// <returns></returns>
inline mat4 rotationaxis(const vec3& v, const float rad) {
	mat4 res = mat4::zero();
	mat4 id = mat4::identity();
	float cosine = cosf(rad);
	float sine = sinf(rad);

	// cross product matrix of v
	mat4 vcross{ 
		{   0,  v.z, -v.y, 0},
		{-v.z, 0   ,  v.x, 0},
		{ v.y, -v.x,    0, 0},
		{   0,    0,    0, 0}
	};

	// outer product of v
	mat4 voutprod{ 
		{v.x * v.x, v.x * v.y, v.x * v.z, 0}, 
		{v.y * v.x, v.y * v.y, v.y * v.z, 0},
		{v.z * v.x, v.z * v.y, v.z * v.z, 0},
		{        0,         0,         0, 0}
	};

	// used formula
	// (cos rad) * Identity + (sin rad) * cross product matrix of v + (1 - cos rad) * outer product of v
	id = id * cosine;
	id[3][3] = 1;
	vcross = vcross * sine;
	voutprod = voutprod * (1 - cosine);

	res = id + vcross + voutprod;

	return res;
}

inline mat4 perspective(const float fovy, const float aspect, const float near, const float far) {
	float scaley = 1 / tanf(fovy / 2);
	float scalex = scaley / aspect;
	float diff = far - near;
#ifdef USE_LH
	float coef = 1;
#else
	float coef = -1;
#endif
	mat4 proj{
		{scalex,      0,                        0,    0},
		{     0, scaley,                        0,    0},
		{     0,      0, coef*(far + near) / diff, coef},
		{     0,      0,   -2 * far * near / diff,    0}
	};
	return proj;
}

inline mat4 lookat(const vec3& eye, const vec3& at, const vec3& up) {
#ifdef USE_LH
	vec3 f = normalize(at - eye); // camera forward
#else
	vec3 f = normalize(eye - at); // camera forward
#endif
	vec3 r = normalize(cross(up, f));    // camera right
	vec3 u = normalize(cross(f, r));     // camera up
	mat4 mat{
		{         r.x,          r.y,          r.z, 0},
		{         u.x,          u.y,          u.z, 0},
		{         f.x,          f.y,          f.z, 0},
		{-dot(eye, r), -dot(eye, u), -dot(eye, f), 1}
	};
	return mat;
}