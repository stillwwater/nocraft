#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <cstdint>
#include <glad/glad.h>

#include "xmath.h"

enum Voxel : uint8_t {
    Voxel_Air,
    Voxel_Grass,
    Voxel_Stone,
};

constexpr int Voxel_Types = 3;

struct Mesh {
    std::vector<Vector3> vertices;
    std::vector<Vector4> colors;
};

constexpr int Chunk_SizeX = 16;
constexpr int Chunk_SizeY = 256;
constexpr int Chunk_SizeZ = 16;

constexpr Vector4 Voxel_ColorMap[Voxel_Types] = {
    {0.00f, 0.00f, 0.00f, 0.00f},
    {0.22f, 0.54f, 0.18f, 1.00f},
    {0.53f, 0.53f, 0.53f, 1.00f},
};

struct Chunk {
    Voxel voxels[Chunk_SizeX][Chunk_SizeY][Chunk_SizeZ]{};
    Mesh mesh;

    // Position in chunk space
    Point3 position;

    GLuint VAO;
    GLuint VBO;
    GLuint color_buffer;

    Chunk(const Point3 &position);

    Chunk(const Chunk &) = delete;
    Chunk &operator=(const Chunk &) = delete;

    ~Chunk();

    void build_chunk_mesh();

    Vector3 world_position() const;
};

inline Vector3 Chunk::world_position() const {
    return Vector3(position) * Vector3(Chunk_SizeX, Chunk_SizeY, Chunk_SizeZ);
}

Voxel make_voxel(const Vector3 &position);

Chunk *load_chunk(const Point3 &position);

void unload_chunk(Chunk *chunk);

#endif // CHUNK_H
