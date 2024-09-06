#pragma once
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/mat4.h"
#include <cmath>

/// <summary>
/// max of two vec3
/// </summary>
/// <param name="a">const vec3&</param>
/// <param name="b">const vec3&</param>
/// <returns>vec3</returns>
inline vec3 max(const vec3& a, const vec3& b)
{
    return vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));
}

/// <summary>
/// max of two vec4
/// </summary>
/// <param name="a">const vec4&</param>
/// <param name="b">const vec4&</param>
/// <returns>vec4</returns>
inline vec4 max(const vec4& a, const vec4& b)
{
    return vec4(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z), fmax(a.w, b.w));
}

/// <summary>
/// compare less for vec3
/// </summary>
/// <param name="a">const vec3&</param>
/// <param name="b">const vec3&</param>
/// <returns>bool</returns>
inline bool cmple(const vec3& a, const vec3& b)
{
    return a.x <= b.x && a.y <= b.y && a.z <= b.z;
}

/// <summary>
/// compare less for vec4
/// </summary>
/// <param name="a">const vec4&</param>
/// <param name="b">const vec4&</param>
/// <returns>bool</returns>
inline bool cmple(const vec4& a, const vec4& b)
{
    return a.x <= b.x && a.y <= b.y && a.z <= b.z && a.w <= b.w;
}

/// <summary>
/// near equal for float
/// </summary>
/// <param name="f0">float</param>
/// <param name="f1">float</param>
/// <param name="tol">float</param>
/// <returns>bool</returns>
inline bool n_fequal(float f0, float f1, float tol)
{
    float f = f0 - f1;
    return ((f > (-tol)) && (f < tol));
}

/// <summary>
/// near equal for vec3
/// </summary>
/// <param name="v0">const vec3&</param>
/// <param name="v1">const vec3&</param>
/// <param name="epsilon">const vec3&</param>
/// <returns>bool</returns>
inline bool nearequal(const vec3& v0, const vec3& v1, const vec3& epsilon)
{
	vec3 delta = v0 - v1;
	vec3 temp = vec3(0,0,0);
	temp = temp - delta;
	temp = max(temp, delta);
	return cmple(temp, epsilon);
}

/// <summary>
/// near equal for vec4
/// </summary>
/// <param name="v0">const vec4&</param>
/// <param name="v1">const vec4&</param>
/// <param name="epsilon">const vec4&</param>
/// <returns>bool</returns>
inline bool nearequal(const vec4& v0, const vec4& v1, const vec4& epsilon)
{
	vec4 delta = v0 - v1;
	vec4 temp = vec4(0,0,0,0);
	temp = temp - delta;
	temp = max(temp, delta);
	return cmple(temp, epsilon);
}

/// <summary>
/// near equal for mat4
/// </summary>
/// <param name="lhs">const mat4&</param>
/// <param name="rhs">const mat4&</param>
/// <returns>bool</returns>
inline bool matnearequal(const mat4& lhs, const mat4& rhs)
{
	const float E = 0.00001f;
    const vec4 E4(E, E, E, E);
	return nearequal(lhs[0], rhs[0], E4) && nearequal(lhs[1], rhs[1], E4) && nearequal(lhs[2], rhs[2], E4) && nearequal(lhs[3], rhs[3], E4);
}
