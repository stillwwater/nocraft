#pragma once

#include "vec.h"

namespace math {

// A matrix with N columns and M rows
template <size_t N, size_t M, typename T>
struct Matrix;

template <typename T>
struct Matrix<4, 4, T> {
    using col_t = Vector<4, T>;
    using row_t = Vector<4, T>;

    col_t values[4];

    Matrix() = default;

    explicit constexpr Matrix(T scalar)
        : values{col_t(scalar, T(0), T(0), T(0)),
                 col_t(T(0), scalar, T(0), T(0)),
                 col_t(T(0), T(0), scalar, T(0)),
                 col_t(T(0), T(0), T(0), scalar)} {}

    constexpr Matrix(const col_t &c0,
                     const col_t &c1,
                     const col_t &c2,
                     const col_t &c3)
        : values{c0, c1, c2, c3} {}

    // Initialize the matrix in row-major order
    constexpr Matrix(T m00, T m01, T m02, T m03,
                     T m10, T m11, T m12, T m13,
                     T m20, T m21, T m22, T m23,
                     T m30, T m31, T m32, T m33)
        : values{col_t(m00, m10, m20, m30),
                 col_t(m01, m11, m21, m31),
                 col_t(m02, m12, m22, m32),
                 col_t(m03, m13, m23, m33)} {}

    constexpr size_t size() const { return 4; }

    // Returns the column at index i. Access with m[column][row]
    col_t &operator[](size_t i) { return values[i]; }
    col_t operator[](size_t i) const { return values[i]; }
};

template <size_t N, size_t M, typename T>
bool operator==(const Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    for (size_t c = 0; c < a.size(); ++c) {
        if (a[c] != b[c]) return false;
    }
    return true;
}

template <size_t N, size_t M, typename T>
bool operator!=(const Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    return !(a == b);
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+(const Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    Matrix<N, M, T> m;
    for (size_t c = 0; c < a.size(); ++c) {
        m[c] = a[c] + b[c];
    }
    return m;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-(const Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    Matrix<N, M, T> m;
    for (size_t c = 0; c < a.size(); ++c) {
        m[c] = a[c] - b[c];
    }
    return m;
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator+=(Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    return (a = a + b);
}

template <size_t N, size_t M, typename T>
Matrix<N, M, T> operator-=(Matrix<N, M, T> &a, const Matrix<N, M, T> &b) {
    return (a = a - b);
}

//
// Multiply
//

template <typename T>
Matrix<4, 4, T> operator*(const Matrix<4, 4, T> &a, const Matrix<4, 4, T> &b) {
    return Matrix<4, 4, T>(
        a[0] * b[0][0] + a[1] * b[0][1] + a[2] * b[0][2] + a[3] * b[0][3],
        a[0] * b[1][0] + a[1] * b[1][1] + a[2] * b[1][2] + a[3] * b[1][3],
        a[0] * b[2][0] + a[1] * b[2][1] + a[2] * b[2][2] + a[3] * b[2][3],
        a[0] * b[3][0] + a[1] * b[3][1] + a[2] * b[3][2] + a[3] * b[3][3]);
}

template <typename T>
typename Matrix<4, 4, T>::col_t operator*(const Matrix<4, 4, T> &m,
                                          const typename Matrix<4, 4, T>::col_t &v) {
    return Matrix<4, 4, T>::col_t(
        m[0][0] * v[0] + m[1][0] * v[1] + m[2][0] * v[2] + m[3][0] * v[3],
        m[0][1] * v[0] + m[1][1] * v[1] + m[2][1] * v[2] + m[3][1] * v[3],
        m[0][2] * v[0] + m[1][2] * v[1] + m[2][2] * v[2] + m[3][2] * v[3],
        m[0][3] * v[0] + m[1][3] * v[1] + m[2][3] * v[2] + m[3][3] * v[3]);
}

template <typename T>
Matrix<4, 4, T> operator*=(Matrix<4, 4, T> &a, const Matrix<4, 4, T> &b) {
    return (a = a * b);
}

} // namespace math
