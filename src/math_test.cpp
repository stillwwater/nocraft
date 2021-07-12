
#include <cstdio>
#include <cassert>

#include "xmath.h"

using namespace math;

template <typename T>
void test_operators() {
    assert((Vector<2, T>(0) == Vector<2, T>(0, 0)));
    assert((Vector<3, T>(0) == Vector<3, T>(0, 0, 0)));
    assert((Vector<4, T>(0) == Vector<4, T>(0, 0, 0, 0)));

    assert((-Vector<2, T>(1) == Vector<2, T>(-1, -1)));
    assert((-Vector<3, T>(1) == Vector<3, T>(-1, -1, -1)));
    assert((-Vector<4, T>(1) == Vector<4, T>(-1, -1, -1, -1)));

    assert((Vector<2, T>(3) + Vector<2, T>(2) == Vector<2, T>(5)));
    assert((Vector<3, T>(3) + Vector<3, T>(2) == Vector<3, T>(5)));
    assert((Vector<4, T>(3) + Vector<4, T>(2) == Vector<4, T>(5)));

    assert((Vector<2, T>(3) - Vector<2, T>(2) == Vector<2, T>(1)));
    assert((Vector<3, T>(3) - Vector<3, T>(2) == Vector<3, T>(1)));
    assert((Vector<4, T>(3) - Vector<4, T>(2) == Vector<4, T>(1)));

    assert((Vector<2, T>(3) * Vector<2, T>(2) == Vector<2, T>(6)));
    assert((Vector<3, T>(3) * Vector<3, T>(2) == Vector<3, T>(6)));
    assert((Vector<4, T>(3) * Vector<4, T>(2) == Vector<4, T>(6)));

    assert((Vector<2, T>(3) / Vector<2, T>(2) == Vector<2, T>(T(1.5))));
    assert((Vector<3, T>(3) / Vector<3, T>(2) == Vector<3, T>(T(1.5))));
    assert((Vector<4, T>(3) / Vector<4, T>(2) == Vector<4, T>(T(1.5))));

    assert((Vector<2, T>(3, 3) + T(6) == Vector<2, T>(9, 9)));
    assert((Vector<3, T>(3, 3, 3) + T(6) == Vector<3, T>(9, 9, 9)));
    assert((Vector<4, T>(3, 3, 3, 3) + T(6) == Vector<4, T>(9, 9, 9, 9)));

    assert((Vector<2, T>(9, 9) - T(6) == Vector<2, T>(3, 3)));
    assert((Vector<3, T>(9, 9, 9) - T(6) == Vector<3, T>(3, 3, 3)));
    assert((Vector<4, T>(9, 9, 9, 9) - T(6) == Vector<4, T>(3, 3, 3, 3)));

    assert((Vector<2, T>(3, 3) * T(3) == Vector<2, T>(9, 9)));
    assert((Vector<3, T>(3, 3, 3) * T(3) == Vector<3, T>(9, 9, 9)));
    assert((Vector<4, T>(3, 3, 3, 3) * T(3) == Vector<4, T>(9, 9, 9, 9)));

    assert((Vector<2, T>(9, 9) / T(3) == Vector<2, T>(3, 3)));
    assert((Vector<3, T>(9, 9, 9) / T(3) == Vector<3, T>(3, 3, 3)));
    assert((Vector<4, T>(9, 9, 9, 9) / T(3) == Vector<4, T>(3, 3, 3, 3)));

    assert((Vector<2, T>(1, 2)[1] == 2));
    assert((Vector<3, T>(1, 2, 3)[1] == 2));
    assert((Vector<4, T>(1, 2, 3, 4)[1] == 2));
}

void test_swizzle() {
    Vector2 v(1, 2);
    v = swizzle<1, 0>(v);
}

void test_mat_operators() {
    Matrix<4, 4, float> m1(
        1 , 2 , 3 , 4 ,
        5 , 6 , 7 , 8 ,
        9 , 10, 11, 12,
        13, 14, 15, 16
    );
    Matrix<4, 4, float> m2 = m1;

    Matrix<4, 4, float>m1m2_mul(
        90 , 100, 110, 120,
        202, 228, 254, 280,
        314, 356, 398, 440,
        426, 484, 542, 600
    );

    assert((m1 * m2 == m1m2_mul));
}

#ifdef TEST

int main(int, char *[]) {
    test_operators<int>();
    test_operators<float>();
    test_mat_operators();
    test_swizzle();
}

#endif
