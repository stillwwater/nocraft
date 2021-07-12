#ifndef RANDOM_H
#define RANDOM_H

#include <cstdint>
#include <cassert>

#include "xmath.h"

// Implementation of xorshift*. Xorshift* has low linear complexity in
// the lower bits which are discarded when generating floats and 32 bit
// intergers. Uses a single 64bit integer for the state so it is cheap
// to copy this struct.
struct Xorshift64 {
    uint64_t state;

    // Seed value chosen completly at random
    Xorshift64(uint64_t seed = 5);

    // Returns a uniformly random int64
    uint64_t nexti64();

    // Returns a uniformly random int32
    uint32_t nexti();

    // Returns a uniformly random double in the range [0, 1)
    double nextf64();

    // Returns a uniformly random float in the range [0, 1)
    float nextf();

    // Returns a random vector with all components in the range [0, 1)
    Vector2 next2();

    // Returns a random vector with all components in the range [0, 1)
    Vector3 next3();

    // Returns a random vector with all components in the range [0, 1)
    Vector4 next4();

    // Returns a uniformly random double in the range [a, b)
    double nextf64(double a, double b);

    // Returns a uniformly random float in the range [a, b)
    float nextf(float a, float b);

    // Returns a random vector with all components in the range [a, b)
    Vector2 next2(const Vector2 &a, const Vector2 &b);

    // Returns a random vector with all components in the range [a, b)
    Vector3 next3(const Vector3 &a, const Vector3 &b);

    // Returns a random vector with all components in the range [a, b)
    Vector4 next4(const Vector4 &a, const Vector4 &b);

    // Returns a random 2D vector with length less than 1
    Vector2 in_unit_circle();

    // Returns a random 3D vector with length less than 1
    Vector3 in_unit_sphere();
};

// Used to initialize the xorshift64 state
struct SplitMix64 {
    uint64_t state;

    SplitMix64(uint64_t seed) : state{seed} {}

    uint64_t nexti64();
};

// 2D simplex noise.
float snoise(const Vector2 &v);

// 3D simplex noise.
float snoise(const Vector3 &v);

// 4D simplex noise.
float snoise(const Vector4 &v);

// 2D simplex fractal noise.
// Noise frequency is multiplied by `lacunarity` for each octave
// `gain` controls how much each octave contributes to the final output.
float snoise_fractal(Vector2 v, int octaves, float lacunarity, float gain);

// 3D simplex fractal noise.
// Noise frequency is multiplied by `lacunarity` for each octave
// `gain` controls how much each octave contributes to the final output.
float snoise_fractal(Vector3 v, int octaves, float lacunarity, float gain);

// 3D simplex billow fractal noise.
float snoise_fractal_b(Vector2 v, int octaves, float lacunarity, float gain);

// 2D simplex billow fractal noise.
float snoise_fractal_b(Vector3 v, int octaves, float lacunarity, float gain);

// Sets the seed for the simplex noise generator functions by shuffling
// values in the permutation table.
void noise_seed(uint64_t seed);

inline uint64_t SplitMix64::nexti64() {
    uint64_t s = state;
    state = s + 0x9e3779b97f4a7c15;
    s = (s ^ (s >> 30)) * 0xbf58476d1ce4e5b9;
    s = (s ^ (s >> 27)) * 0x94d049bb133111eb;
    return s ^ (s >> 31);
}

inline Xorshift64::Xorshift64(uint64_t seed) {
    SplitMix64 sm{seed};
    this->state = sm.nexti64();
}

inline uint64_t Xorshift64::nexti64() {
    assert(state != 0 && "Xorshift: Invalid state");
    uint64_t s = state;
    s ^= s >> 12;
    s ^= s << 25;
    s ^= s >> 27;
    state = s;
    return s * UINT64_C(0x2545f4914f6cdd1d);
}

inline uint32_t Xorshift64::nexti() {
    return nexti64() >> 32;
}

inline double Xorshift64::nextf64() {
    // (randi64() >> 11) * 0x1.0p-53
    uint64_t i = (UINT64_C(0x3ff) << 52) | (nexti64() >> 12);
    return (*(double *)&i) - 1.0;
}

inline float Xorshift64::nextf() {
    uint32_t i = 0x3f800000 | (nexti() >> 9);
    return (*(float *)&i) - 1.0f;
}

inline Vector2 Xorshift64::next2() {
    return Vector2{nextf(), nextf()};
}

inline Vector3 Xorshift64::next3() {
    return Vector3{nextf(), nextf(), nextf()};
}

inline Vector4 Xorshift64::next4() {
    return Vector4{nextf(), nextf(), nextf(), nextf()};
}

inline float Xorshift64::nextf(float a, float b) {
    return nextf() * (b - a) + a;
}

inline Vector2 Xorshift64::next2(const Vector2 &a, const Vector2 &b) {
    return next2() * (b - a) + a;
}

inline Vector3 Xorshift64::next3(const Vector3 &a, const Vector3 &b) {
    return next3() * (b - a) + a;
}

inline Vector4 Xorshift64::next4(const Vector4 &a, const Vector4 &b) {
    return next4() * (b - a) + a;
}

#endif // RANDOM_H
