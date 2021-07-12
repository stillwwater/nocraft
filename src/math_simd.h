#ifndef MATH_SIMD_H
#define MATH_SIMD_H

#pragma once

#if defined(__SSE__)
#    define MATH_ARCH_SSE2
#elif defined(_MSC_VER)
#    if defined(_M_AMD64) || defined(_M_X64)
#        define MATH_ARCH_SSE2
#    elif _M_IX86_FP
#        define MATH_ARCH_SSE2
#    endif
#endif

#ifdef MATH_ARCH_SSE2

#include <xmmintrin.h>

#include "vec.h"

namespace math {

template <>
inline Vector<4, float> operator+(const Vector<4, float> &a, const Vector<4, float> &b) {
    __m128 result = _mm_add_ps(*(const __m128 *)&a, *(const __m128 *)&b);
    return *(Vector<4, float> *)&result;
}

template <>
inline Vector<4, float> operator-(const Vector<4, float> &a, const Vector<4, float> &b) {
    __m128 result = _mm_sub_ps(*(const __m128 *)&a, *(const __m128 *)&b);
    return *(Vector<4, float> *)&result;
}

template <>
inline Vector<4, float> operator*(const Vector<4, float> &a, const Vector<4, float> &b) {
    __m128 result = _mm_mul_ps(*(const __m128 *)&a, *(const __m128 *)&b);
    return *(Vector<4, float> *)&result;
}

template <>
inline Vector<4, float> operator/(const Vector<4, float> &a, const Vector<4, float> &b) {
    __m128 result = _mm_div_ps(*(const __m128 *)&a, *(const __m128 *)&b);
    return *(Vector<4, float> *)&result;
}

template <>
inline bool operator==(const Vector<4, float> &a, const Vector<4, float> &b) {
    return _mm_movemask_ps(_mm_cmpeq_ps(*(const __m128 *)&a, *(const __m128 *)&b)) != 0;
}

} // namespace math

#endif

#endif // MATH_SIMD_H
