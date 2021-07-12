#ifndef XMATH_H
#define XMATH_H

#pragma once

#include <cmath>
#include <limits>

#include "vec.h"
#include "matrix.h"
#include "math_simd.h"

#ifndef real_t
// Default floating point type
#define real_t float
#endif

namespace math {

static_assert(std::numeric_limits<real_t>::is_iec559, "'real_t' must be a floating-point type");

//
// Constants
//

constexpr real_t Epsilon = real_t(std::numeric_limits<real_t>::epsilon());
constexpr real_t PI      = real_t(3.141592653589793238463);
constexpr real_t PIOver2 = real_t(1.570796326794896619231);
constexpr real_t PIOver4 = real_t(0.785398163397448309615);
constexpr real_t TwoPI   = real_t(6.283185307179586476925);
constexpr real_t e       = real_t(2.718281828459045235360);

//
// Common
//

template <typename T>
T min(T a, T b) {
    static_assert(std::numeric_limits<T>::is_iec559 || std::numeric_limits<T>::is_integer,
                  "'min' requires numeric types");
    return (b < a) ? b : a;
}

template <typename T>
T max(T a, T b) {
    static_assert(std::numeric_limits<T>::is_iec559 || std::numeric_limits<T>::is_integer,
                  "'max' requires numeric types");
    return (a < b) ? b : a;
}

template <typename T>
T sign(T x) {
    return (T(0) < x) - (x < T(0));
}

template <typename T>
T clamp(T x, T a, T b) {
    return min(max(x, a), b);
}

template <typename T>
T saturate(T x) {
    static_assert(std::numeric_limits<T>::is_iec559, "'saturate' requires floating-point types");
    return clamp(x, T(0), T(1));
}

template <typename T>
T lerp(T a, T b, T t) {
    static_assert(std::numeric_limits<T>::is_iec559, "'lerp' requires floating-point types");
    return a + (b - a) * t;
}

template <size_t N, typename T, typename U>
Vector<N, T> lerp(const Vector<N, T> &a, const Vector<N, T> &b, U t) {
    static_assert(std::numeric_limits<T>::is_iec559 && std::numeric_limits<U>::is_iec559,
                  "'lerp' requires floating-point types");
    return a + (b - a) * T(t);
}

template <typename T>
T radians(T degrees) {
    static_assert(std::numeric_limits<T>::is_iec559, "'radians' requires floating-point types");
    return degrees * T(0.01745329251994329576923690768489);
}

template <typename T>
T degrees(T radians) {
    static_assert(std::numeric_limits<T>::is_iec559, "'degrees' requires floating-point types");
    return radians * T(57.295779513082320876798154814105);
}

//
// Geometric Functions
//

template <typename T>
T dot(const Vector<2, T> &a, const Vector<2, T> &b) {
    static_assert(std::numeric_limits<T>::is_iec559, "'dot' requires floating-point types");
    return a.x * b.x + a.y * b.y;
}

template <typename T>
T dot(const Vector<3, T> &a, const Vector<3, T> &b) {
    static_assert(std::numeric_limits<T>::is_iec559, "'dot' requires floating-point types");
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T>
T dot(const Vector<4, T> &a, const Vector<4, T> &b) {
    static_assert(std::numeric_limits<T>::is_iec559, "'dot' requires floating-point types");
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <size_t N, typename T>
T length(const Vector<N, T> &v) {
    static_assert(std::numeric_limits<T>::is_iec559, "'length' requires floating-point types");
    return std::sqrt(dot(v, v));
}

template <size_t N, typename T>
T length2(const Vector<N, T> &v) {
    static_assert(std::numeric_limits<T>::is_iec559, "'length2' requires floating-point types");
    return dot(v, v);
}

template <size_t N, typename T>
T distance(const Vector<N, T> &a, const Vector<N, T> &b) {
    static_assert(std::numeric_limits<T>::is_iec559, "'distance' requires floating-point types");
    return length(a - b);
}

template <size_t N, typename T>
Vector<N, T> normalize(const Vector<N, T> &v) {
    static_assert(std::numeric_limits<T>::is_iec559, "'normalize' requires floating-point types");
    return v / length(v);
}

template <size_t N, typename T>
T normalize(Vector<N, T> *v) {
    static_assert(std::numeric_limits<T>::is_iec559, "'normalize' requires floating-point types");
    T l = length(*v);
    *v /= l;
    return l;
}

template <size_t N, typename T>
Vector<N, T> reflect(const Vector<N, T> &I, const Vector<N, T> n) {
    return I - n * dot(n, I) * T(2);
}

template <typename T>
Vector<3, T> cross(const Vector<3, T> &a, const Vector<3, T> &b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

//
// Swizzle
//

template <size_t X, size_t Y, size_t N, typename T>
Vector<2, T> swizzle(const Vector<N, T> &v) {
    return Vector<2, T>(v[X], v[Y]);
}

template <size_t X, size_t Y, size_t Z, size_t N, typename T>
Vector<3, T> swizzle(const Vector<N, T> &v) {
    return Vector<3, T>(v[X], v[Y], v[Z]);
}

template <size_t X, size_t Y, size_t Z, size_t W, size_t N, typename T>
Vector<4, T> swizzle(const Vector<N, T> &v) {
    return Vector<4, T>(v[X], v[Y], v[Z], v[W]);
}

//
// Matrix projections
//

template <typename T>
Matrix<4, 4, T> perspective(T fovy, T aspect, T znear, T zfar) {
    const T h = tan(fovy / T(2));

    Matrix<4, 4, T> m(T(0));
    m[0][0] = T(1) / (aspect * h);
    m[1][1] = T(1) / h;
    m[2][2] = -(zfar + znear) / (zfar - znear);
    m[2][3] = -T(1);
    m[3][2] = -(T(2) * zfar * znear) / (zfar - znear);
    return m;
}

// Right handed
template <typename T>
Matrix<4, 4, T> lookat(const Vector<3, T> &eye,
                       const Vector<3, T> &target,
                       const Vector<3, T> &up) {

    auto d = normalize(target - eye);
    auto r = normalize(cross(d, up));
    auto u = normalize(cross(r, d));

    return Matrix<4, 4, T>(
        Vector<4, T>(r.x, u.x, -d.x, T(0)),
        Vector<4, T>(r.y, u.y, -d.y, T(0)),
        Vector<4, T>(r.z, u.z, -d.z, T(0)),
        Vector<4, T>(-dot(r, eye), -dot(u, eye), dot(d, eye), T(1)));
}

//
// Matrix transforms
//

template <typename T>
Matrix<4, 4, T> translate(const Matrix<4, 4, T> &m, const Vector<3, T> &v) {
    return Matrix<4, 4, T>(
        m[0],
        m[1],
        m[2],
        m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3]);
}

template <typename T>
Matrix<4, 4, T> scale(const Matrix<4, 4, T> &m, const Vector<3, T> &v) {
    return Matrix<4, 4, T>(
        m[0] * v[0],
        m[1] * v[1],
        m[2] * v[2],
        m[3]);
}

} // namespace math

#ifndef MATH_NO_ALIASES
using Point2 = math::Vector<2, int>;
using Point3 = math::Vector<3, int>;
using Point4 = math::Vector<4, int>;

using Vector2 = math::Vector<2, real_t>;
using Vector3 = math::Vector<3, real_t>;
using Vector4 = math::Vector<4, real_t>;

using Matrix4 = math::Matrix<4, 4, real_t>;
#endif

#endif // XMATH_H
