#include "random.h"

#include <cmath>
#include <cstring>
#include <algorithm>

#include "xmath.h"

static const Vector3 grad3[] = {
    {1, 1, 0}, {-1, 1, 0}, {1, -1, 0}, {-1, -1, 0},
    {1, 0, 1}, {-1, 0, 1}, {1, 0, -1}, {-1, 0, -1},
    {0, 1, 1}, {0, -1, 1}, {0, 1, -1}, {0, -1, -1},
};

static const Vector4 grad4[] = {
    { 0,  1, 1, 1}, { 0,  1,  1, -1}, { 0,  1, -1, 1}, { 0,  1, -1, -1},
    { 0, -1, 1, 1}, { 0, -1,  1, -1}, { 0, -1, -1, 1}, { 0, -1, -1, -1},
    { 1,  0, 1, 1}, { 1,  0,  1, -1}, { 1,  0, -1, 1}, { 1,  0, -1, -1},
    {-1,  0, 1, 1}, {-1,  0,  1, -1}, {-1,  0, -1, 1}, {-1,  0, -1, -1},
    { 1,  1, 0, 1}, { 1,  1,  0, -1}, { 1, -1,  0, 1}, { 1, -1,  0, -1},
    {-1,  1, 0, 1}, {-1,  1,  0, -1}, {-1, -1,  0, 1}, {-1, -1,  0, -1},
    { 1,  1, 1, 0}, { 1,  1, -1,  0}, { 1, -1,  1, 0}, { 1, -1, -1,  0},
    {-1,  1, 1, 0}, {-1,  1, -1,  0}, {-1, -1,  1, 0}, {-1, -1, -1,  0},
};

static unsigned char perm[512] = {
    151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
    140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
    247,120,234, 75,  0, 26,197, 62, 94,252,219,203,117, 35, 11, 32,
     57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
     74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
     60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
     65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
    200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
     52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
    207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
    119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
    129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
    218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
     81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
    184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
    222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180,

    // And again so it wraps nicely
    151,160,137, 91, 90, 15,131, 13,201, 95, 96, 53,194,233,  7,225,
    140, 36,103, 30, 69,142,  8, 99, 37,240, 21, 10, 23,190,  6,148,
    247,120,234, 75,  0, 26,197, 62, 94,252,219,203,117, 35, 11, 32,
     57,177, 33, 88,237,149, 56, 87,174, 20,125,136,171,168, 68,175,
     74,165, 71,134,139, 48, 27,166, 77,146,158,231, 83,111,229,122,
     60,211,133,230,220,105, 92, 41, 55, 46,245, 40,244,102,143, 54,
     65, 25, 63,161,  1,216, 80, 73,209, 76,132,187,208, 89, 18,169,
    200,196,135,130,116,188,159, 86,164,100,109,198,173,186,  3, 64,
     52,217,226,250,124,123,  5,202, 38,147,118,126,255, 82, 85,212,
    207,206, 59,227, 47, 16, 58, 17,182,189, 28, 42,223,183,170,213,
    119,248,152,  2, 44,154,163, 70,221,153,101,155,167, 43,172,  9,
    129, 22, 39,253, 19, 98,108,110, 79,113,224,232,178,185,112,104,
    218,246, 97,228,251, 34,242,193,238,210,144, 12,191,179,162,241,
     81, 51,145,235,249, 14,239,107, 49,192,214, 31,181,199,106,157,
    184, 84,204,176,115,121, 50, 45,127,  4,150,254,138,236,205, 93,
    222,114, 67, 29, 24, 72,243,141,128,195, 78, 66,215, 61,156,180
};

static inline int floortoi(float value) {
    int x = int(value);
    return value < x ? x - 1 : x;
}

template <typename T, typename U>
static inline float gradient(const U *grad, int gi, const T &v, float c) {
    float t = c - math::length2(v);
    if (t < 0.0f) return 0.0f;
    t *= t;
    return t * t * math::dot(T(grad[gi]), v);
}

void snoise_seed(uint64_t seed) {
    Xorshift64 rng{seed};
    int len = sizeof(perm) / 2;
    for (int i = len - 1; i > 0; --i) {
        int target = int(rng.nextf(0.0f, float(i) + 1.0f));
        std::swap(perm[i], perm[target]);
    }
    // Duplicate for the other 256 elements
    memcpy(perm + len, perm, len);
}

// Based on the 2D simplex noise code by Stefan Gustavson.
float snoise(const Vector2 &v) {
    constexpr float F2 = 0.366025403784439f;  // 0.5*(sqrt(3.0)-1.0) (skew)
    constexpr float G2 = 0.211324865405187f;  // (3.0-sqrt(3.0))/6.0 (unskew)

    // Skew the input space to determine which simplex cell we're in
    float s = (v.x + v.y) * F2; // Hairy factor for 2D

    // First corner
    Point2 i{floortoi(v.x + s), floortoi(v.y + s)};
    Vector2 t = Vector2((i.x + i.y) * G2);
    Vector2 x0 = v - (Vector2(i) - t);

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    Point2 i1;
    if (x0.x > x0.y)
        i1 = {1, 0};
    else
        i1 = {0, 1};

    // Other corners
    Vector2 x1 = x0 - Vector2(i1) + Vector2(G2);
    Vector2 x2 = x0 - Vector2(1.0) + Vector2(2.0 * G2);

    // Permutations
    Point2 h{i.x & 255, i.y & 255};
    int gi0 = perm[h.x + perm[h.y]] % 12;
    int gi1 = perm[h.x + i1.x + perm[h.y + i1.y]] % 12;
    int gi2 = perm[h.x + 1 + perm[h.y + 1]] % 12;

    // Calculate the contribution from the three corners
    float n0 = gradient(grad3, gi0, x0, 0.5f);
    float n1 = gradient(grad3, gi1, x1, 0.5f);
    float n2 = gradient(grad3, gi2, x2, 0.5f);
    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 70.0f * (n0 + n1 + n2);
}

// Based on the 3D simplex noise code by Stefan Gustavson.
float snoise(const Vector3 &v) {
    constexpr float F3 = 1.0f / 3.0f; // (skew)
    constexpr float G3 = 1.0f / 6.0f; // (unskew)

    // Skew the input space to determine which simplex cell we're in
    float s = (v.x + v.y + v.z) * F3;

    // First corner
    Point3 i{floortoi(v.x + s), floortoi(v.y + s), floortoi(v.z + s)};
    Vector3 t = Vector3((i.x + i.y + i.z) * G3);
    Vector3 x0 = v - (Vector3(i) - t);

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    Point3 i1, i2;
    if(x0.x >= x0.y) {
        if(x0.y >= x0.z) {
            i1 = {1, 0, 0};
            i2 = {1, 1, 0};
        } else if(x0.x >= x0.z) {
            i1 = {1, 0, 0};
            i2 = {1, 0, 1};
        } else {
            i1 = {0, 0, 1};
            i2 = {1, 0, 1};
        }
    } else {
        if(x0.y < x0.z) {
            i1 = {0, 0, 1};
            i2 = {0, 1, 1};
        } else if(x0.x < x0.z) {
            i1 = {0, 1, 0};
            i2 = {0, 1, 1};
        } else {
            i1 = {0, 1, 0};
            i2 = {1, 1, 0};
        }
    }
    // Other corners
    Vector3 x1 = x0 - Vector3(i1) + Vector3(G3);
    Vector3 x2 = x0 - Vector3(i2) + Vector3(2.0f * G3);
    Vector3 x3 = x0 - Vector3(1.0) + Vector3(3.0f * G3);

    // Permutations
    Point3 h{i.x & 255, i.y & 255, i.z & 255};
    int gi0 = perm[h.x + perm[h.y + perm[h.z]]] % 12;
    int gi1 = perm[h.x + i1.x + perm[h.y + i1.y + perm[h.z + i1.z]]] % 12;
    int gi2 = perm[h.x + i2.x + perm[h.y + i2.y + perm[h.z + i2.z]]] % 12;
    int gi3 = perm[h.x + 1 + perm[h.y + 1 + perm[h.z + 1]]] % 12;

    // Contribution from the four corners
    float n0 = gradient(grad3, gi0, x0, 0.6f);
    float n1 = gradient(grad3, gi1, x1, 0.6f);
    float n2 = gradient(grad3, gi2, x2, 0.6f);
    float n3 = gradient(grad3, gi3, x3, 0.6f);

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 32.0f * (n0 + n1 + n2 + n3);
}

// Based on the improved simplex rank ordering method for 4D simplex noise
// by Stefan Gustavson.
float snoise(const Vector4 &v) {
    constexpr float F4 = 0.309016994375947f; // (sqrt(5.0)-1)/4.0 (skew)
    constexpr float G4 = 0.138196601125011f; // (5.0-sqrt(5.0))/20.0 (unskew)

    // Skew the space to determine which cell of 24 simplices we're in
    float s = (v.x + v.y + v.z + v.w) * F4;

    // First corner
    Point4 i = {floortoi(v.x + s), floortoi(v.y + s),
                floortoi(v.z + s), floortoi(v.w + s)};

    Vector4 t = Vector4((i.x + i.y + i.z + i.w) * G4);
    Vector4 x0 = v - (Vector4(i) - t);

    // For the 4D case, the simplex shape is a pentatope instead of
    // a tesseract which is used in classic noise, meaning we only need to
    // calculate 5 vertecies instead of 16. For n dimentions, simplex noise
    // uses n+1 vertecies for the simplex shape, compared to classic noise
    // which requires n^2 vertecies.
    Point4 rank = Point4(0);
    if (x0.x > x0.y) rank.x++; else rank.y++;
    if (x0.x > x0.z) rank.x++; else rank.z++;
    if (x0.x > x0.w) rank.x++; else rank.w++;
    if (x0.y > x0.z) rank.y++; else rank.z++;
    if (x0.y > x0.w) rank.y++; else rank.w++;
    if (x0.z > x0.w) rank.z++; else rank.w++;

    // rank is a vector with the numbers 0, 1, 2 and 3 in some order.
    // We use a thresholding to set the coordinates in turn.
    Point4 i1 = {rank.x >= 3, rank.y >= 3, rank.z >= 3, rank.w >= 3};
    Point4 i2 = {rank.x >= 2, rank.y >= 2, rank.z >= 2, rank.w >= 2};
    Point4 i3 = {rank.x >= 1, rank.y >= 1, rank.z >= 1, rank.w >= 1};

    // Other corners
    Vector4 x1 = x0 - Vector4(i1) + Vector4(G4);
    Vector4 x2 = x0 - Vector4(i2) + Vector4(2.0f * G4);
    Vector4 x3 = x0 - Vector4(i3) + Vector4(3.0f * G4);
    Vector4 x4 = x0 - Vector4(1.0f) + Vector4(4.0f * G4);

    // Permutations
    Point4 h{i.x & 255, i.y & 255, i.z & 255, i.w & 255};
    int gi0 = perm[h.x + perm[h.y + perm[h.z + perm[h.w]]]] % 32;

    int gi1 = perm[h.x + i1.x + perm[h.y + i1.y
            + perm[h.z + i1.z + perm[h.w + i1.w]]]] % 32;

    int gi2 = perm[h.x + i2.x + perm[h.y + i2.y
            + perm[h.z + i2.z + perm[h.w + i2.w]]]] % 32;

    int gi3 = perm[h.x + i3.x + perm[h.y + i3.y
            + perm[h.z + i3.z + perm[h.w + i3.w]]]] % 32;

    int gi4 = perm[h.x + 1 + perm[h.y + 1
            + perm[h.z + 1 + perm[h.w + 1]]]] % 32;

    // Contribution from the four corners
    float n0 = gradient(grad4, gi0, x0, 0.6f);
    float n1 = gradient(grad4, gi1, x1, 0.6f);
    float n2 = gradient(grad4, gi2, x2, 0.6f);
    float n3 = gradient(grad4, gi3, x3, 0.6f);
    float n4 = gradient(grad4, gi4, x4, 0.6f);

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return 27.0f * (n0 + n1 + n2 + n3 + n4);
}

template <typename T>
static inline float fractal_fbm(T v, int octaves, float lac, float gain) {
    float sum = 0.0f;
    float amp = 1.0f;
    float frac_range = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        sum += snoise(v) * amp;
        frac_range += amp;
        amp *= gain;
        v *= lac;
    }
    return sum / frac_range;
}

template <typename T>
static inline float fractal_b(T v, int octaves, float lac, float gain) {
    float sum = 0.0f;
    float amp = 1.0f;
    float frac_range = 0.0f;

    for (int i = 0; i < octaves; ++i) {
        sum += (fabsf(snoise(v)) * 2.0f - 1.0f) * amp;
        frac_range += amp;
        amp *= gain;
        v *= lac;
    }
    return sum / frac_range;
}

float snoise_fractal(Vector3 v, int octaves, float lacunarity, float gain) {
    return fractal_fbm(v, octaves, lacunarity, gain);
}

float snoise_fractal(Vector2 v, int octaves, float lacunarity, float gain) {
    return fractal_fbm(v, octaves, lacunarity, gain);
}

float snoise_fractal_b(Vector3 v, int octaves, float lacunarity, float gain) {
    return fractal_b(v, octaves, lacunarity, gain);
}

float snoise_fractal_b(Vector2 v, int octaves, float lacunarity, float gain) {
    return fractal_b(v, octaves, lacunarity, gain);
}
