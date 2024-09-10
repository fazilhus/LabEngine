#pragma once

#include <cmath>
#include <cstdio>
#include <cassert>

#include "vec3.h"
#include "vec4.h"

class mat4;

mat4 transpose(const mat4& m);

class mat4 {
public:
	vec4 m[4];

	mat4() {
		m[0] = vec4{ 1, 0, 0, 0 };
		m[1] = vec4{ 0, 1, 0, 0 };
		m[2] = vec4{ 0, 0, 1, 0 };
		m[3] = vec4{ 0, 0, 0, 1 };
	}

	mat4(const vec4& r0, const vec4& r1, const vec4& r2, const vec4& r3) {
		m[0] = r0; m[1] = r1; m[2] = r2; m[3] = r3;
	}

	mat4(const mat4& other) {
		for (std::size_t i = 0; i < 4; ++i) {
			m[i] = other[i];
		}
	}

	static mat4 zero() {
		return mat4{
			vec4{},
			vec4{},
			vec4{},
			vec4{}
		};
	}

	static mat4 identity() {
		return mat4{
			vec4{ 1, 0, 0, 0 },
			vec4{ 0, 1, 0, 0 },
			vec4{ 0, 0, 1, 0 },
			vec4{ 0, 0, 0, 1 }
		};
	}

	mat4& operator=(const mat4& other) {
		for (std::size_t i = 0; i < 4; ++i) {
			m[i] = other[i];
		}
		return *this;
	}

	const mat4 operator+(const mat4& rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] + rhs[i][j];
			}
		}
		return res;
	}

	const mat4 operator-(const mat4& rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] - rhs[i][j];
			}
		}
		return res;
	}

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

	const mat4 operator*(const float rhs) const {
		mat4 res = mat4::zero();
		for (std::size_t i = 0; i < 4; ++i) {
			for (std::size_t j = 0; j < 4; ++j) {
				res[i][j] = this->m[i][j] * rhs;
			}
		}
		return res;
	}

	const vec4 operator*(const vec4& rhs) const {
		vec4 res{};
		mat4 thisT = transpose(*this);
		for (std::size_t i = 0; i < 4; ++i) {
			res[i] = dot(thisT[i], rhs);
		}
		return res;
	}

	const bool operator==(const mat4& rhs) const {
		for (std::size_t i = 0; i < 4; ++i) {
			if (this->m[i] != rhs[i]) return false;
		}
		return true;
	}

	const bool operator!=(const mat4& rhs) const {
		return !(*this == rhs);
	}

	vec4& operator[](const std::size_t i) {
		return this->m[i];
	}

	const vec4& operator[](const std::size_t i) const {
		return this->m[i];
	}
};

inline void mat4print(const mat4& m) {
	for (std::size_t i = 0; i < 4; ++i) {
		vec4print(m[i]);
	}
	printf("\n");
}

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

inline mat4 translate(const vec3& v) {
	mat4 res = mat4::identity();
	for (std::size_t i = 0; i < 3; ++i) {
		res[3][i] = v[i];
	}
	return res;
}

inline mat4 scale(const vec3& v) {
	mat4 res = mat4::identity();
	for (std::size_t i = 0; i < 3; ++i) {
		res[i][i] = v[i];
	}
	return res;
}

inline mat4 transpose(const mat4& m) {
	mat4 res = mat4::zero();
	for (std::size_t i = 0; i < 4; ++i) {
		for (std::size_t j = 0; j < 4; ++j) {
			res[i][j] = m[j][i];
		}
	}
	return res;
}

inline mat4 rotationx(const float rad) {
	mat4 res = mat4::zero();
	float cosine = cosf(rad);
	float sine = sinf(rad);
	res[0] = vec4{ 1, 0, 0, 0 };
	res[1] = vec4{ 0, cosine, -sine, 0 };
	res[2] = vec4{ 0, sine, cosine, 0 };
	res[3] = vec4{ 0, 0, 0, 1 };
	return res;
}

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

inline mat4 rotationz(const float rad) {
	mat4 res = mat4::zero();
	float cosine = cosf(rad);
	float sine = sinf(rad);
	res[0] = vec4{ cosine, -sine, 0, 0 };
	res[1] = vec4{ sine, cosine, 0, 0 };
	res[2] = vec4{ 0, 0, 1, 0 };
	res[3] = vec4{ 0, 0, 0, 1 };
	return res;
}

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