#ifndef VEC_H
#define VEC_H

#pragma once

#define MATH_VEC2_OP(op)                                                        \
template <typename T>                                                           \
inline Vector<2, T> operator op(const Vector<2, T> &a, const Vector<2, T> &b) { \
    return {a.x op b.x, a.y op b.y};                                            \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<2, T> operator op(const Vector<2, T> &a, Q scalar) {              \
    return {a.x op scalar, a.y op scalar};                                      \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<2, T> operator op(Q scalar, const Vector<2, T> &a) {              \
    return {scalar op a.x, scalar op a.y};                                      \
}

#define MATH_VEC3_OP(op)                                                        \
template <typename T>                                                           \
inline Vector<3, T> operator op(const Vector<3, T> &a, const Vector<3, T> &b) { \
    return {a.x op b.x, a.y op b.y, a.z op b.z};                                \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<3, T> operator op(const Vector<3, T> &a, Q scalar) {              \
    return {a.x op scalar, a.y op scalar, a.z op scalar};                       \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<3, T> operator op(Q scalar, const Vector<3, T> &a) {              \
    return {scalar op a.x, scalar op a.y, scalar op a.z};                       \
}

#define MATH_VEC4_OP(op)                                                        \
template <typename T>                                                           \
inline Vector<4, T> operator op(const Vector<4, T> &a, const Vector<4, T> &b) { \
    return {a.x op b.x, a.y op b.y, a.z op b.z, a.w op b.w};                    \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<4, T> operator op(const Vector<4, T> &a, Q scalar) {              \
    return {a.x op scalar, a.y op scalar, a.z op scalar, a.w op scalar};        \
}                                                                               \
                                                                                \
template <typename T, typename Q>                                               \
inline Vector<4, T> operator op(Q scalar, const Vector<4, T> &a) {              \
    return {scalar op a.x, scalar op a.y, scalar op a.z, scalar op a.w};        \
}

#define MATH_VEC_OP_ASSIGN(op)                                                  \
template <size_t N, typename T>                                                 \
inline Vector<N, T> operator op##=(Vector<N, T> &a, const Vector<N, T> &b) {    \
    a = a op b;                                                                 \
    return a;                                                                   \
}                                                                               \
                                                                                \
template <size_t N, typename T, typename Q>                                     \
inline Vector<N, T> operator op##=(Vector<N, T> &a, Q scalar) {                 \
    a = a op scalar;                                                            \
    return a;                                                                   \
}

namespace math {

template <size_t N, typename T>
struct Vector;

template <typename T>
struct Vector<2, T> {
    T x, y;

    Vector() = default;

    constexpr Vector(T x, T y) : x(x), y(y) {}
    explicit constexpr Vector(T scalar) : x(scalar), y(scalar) {}

    template <typename Q>
    explicit Vector(const Vector<2, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y)) {}

    template <typename Q>
    explicit Vector(const Vector<3, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y)) {}

    template <typename Q>
    explicit Vector(const Vector<4, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y)) {}

    constexpr size_t size() const { return 2; }

    T operator[](size_t i) const { return ((const T *)this)[i]; }
    T &operator[](size_t i) { return ((T *)this)[i]; }

    Vector operator+() const { return *this; };
    Vector operator-() const { return Vector(-x, -y); };
};

MATH_VEC2_OP(+)
MATH_VEC2_OP(-)
MATH_VEC2_OP(*)
MATH_VEC2_OP(/)

template <typename T>
bool operator==(const Vector<2, T> &a, const Vector<2, T> &b) {
    return a.x == b.x && a.y == a.y;
}

template <typename T>
bool operator!=(const Vector<2, T> &a, const Vector<2, T> &b) {
    return !(a == b);
}

template <typename T>
struct Vector<3, T> {
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };

    Vector() = default;

    constexpr Vector(T x, T y, T z) : x(x), y(y), z(z) {}
    explicit constexpr Vector(T scalar) : x(scalar), y(scalar), z(scalar) {}

    template <typename Q>
    explicit Vector(const Vector<2, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(0)) {}

    template <typename Q>
    explicit Vector(const Vector<3, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z)) {}

    template <typename Q>
    explicit Vector(const Vector<4, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z)) {}

    constexpr size_t size() const { return 3; }

    T operator[](size_t i) const { return ((const T *)this)[i]; }
    T &operator[](size_t i) { return ((T *)this)[i]; }

    Vector operator+() const { return *this; };
    Vector operator-() const { return Vector(-x, -y, -z); }
};

MATH_VEC3_OP(+)
MATH_VEC3_OP(-)
MATH_VEC3_OP(*)
MATH_VEC3_OP(/)

template <typename T>
bool operator==(const Vector<3, T> &a, const Vector<3, T> &b) {
    return a.x == b.x && a.y == a.y && a.z == b.z;
}

template <typename T>
bool operator!=(const Vector<3, T> &a, const Vector<3, T> &b) {
    return !(a == b);
}

template <typename T>
struct Vector<4, T> {
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };

    Vector() = default;

    constexpr Vector(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    explicit constexpr Vector(T scalar)
        : x(scalar), y(scalar), z(scalar), w(scalar) {}

    template <typename Q>
    explicit Vector(const Vector<2, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(0))
        , w(static_cast<T>(0)) {}

    template <typename Q>
    explicit Vector(const Vector<3, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z))
        , w(static_cast<T>(0)) {}

    template <typename Q>
    explicit Vector(const Vector<4, Q> &v)
        : x(static_cast<T>(v.x))
        , y(static_cast<T>(v.y))
        , z(static_cast<T>(v.z))
        , w(static_cast<T>(v.w)) {}

    constexpr size_t size() const { return 4; }

    T operator[](size_t i) const { return ((const T *)this)[i]; }
    T &operator[](size_t i) { return ((T *)this)[i]; }

    Vector operator+() const { *this; }
    Vector operator-() const { return Vector(-x, -y, -z, -w); }
};

MATH_VEC4_OP(+)
MATH_VEC4_OP(-)
MATH_VEC4_OP(*)
MATH_VEC4_OP(/)

template <typename T>
bool operator==(const Vector<4, T> &a, const Vector<4, T> &b) {
    return a.x == b.x && a.y == a.y && a.z == b.z && a.w == b.w;
}

template <typename T>
bool operator!=(const Vector<4, T> &a, const Vector<4, T> &b) {
    return !(a == b);
}

MATH_VEC_OP_ASSIGN(+)
MATH_VEC_OP_ASSIGN(-)
MATH_VEC_OP_ASSIGN(*)
MATH_VEC_OP_ASSIGN(/)

} // namespace math

#undef MATH_VEC2_OP
#undef MATH_VEC3_OP
#undef MATH_VEC4_OP
#undef MATH_VEC_OP_ASSIGN

#endif // VEC_H
