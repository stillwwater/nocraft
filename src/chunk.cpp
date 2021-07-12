#include "chunk.h"

#include <vector>
#include <glad/glad.h>

#include "xmath.h"
#include "random.h"

void unload_chunk(Chunk *chunk) {
    delete chunk;
}

Chunk::Chunk(const Point3 &position) : position{position} {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &color_buffer);
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &color_buffer);
}

void Chunk::build_chunk_mesh() {
    auto &verts = mesh.vertices;
    verts.clear();

    for (int x = 0; x < Chunk_SizeX; ++x) {
        for (int y = 0; y < Chunk_SizeY; ++y) {
            for (int z = 0; z < Chunk_SizeZ; ++z) {
                auto voxel = voxels[x][y][z];
                if (voxel == Voxel_Air) {
                    continue;
                }

                auto pos = Vector3(float(x), float(y), float(z));
                int n_verts = 0;

                // Front face (towards +Z)
                if (z == Chunk_SizeZ - 1 || voxels[x][y][z + 1] == Voxel_Air) {
                    verts.push_back(pos + Vector3(-0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3( 0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f,  0.5f));
                    n_verts += 6;
                }

                // Back face (towards -Z)
                if (z == 0 || voxels[x][y][z - 1] == Voxel_Air) {
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    n_verts += 6;
                }

                // Left face (towards -X)
                if (x == 0 || voxels[x - 1][y][z] == Voxel_Air) {
                    verts.push_back(pos + Vector3(-0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f,  0.5f));
                    n_verts += 6;
                }

                // Right face (towards +X)
                if (x == Chunk_SizeX - 1 || voxels[x + 1][y][z] == Voxel_Air) {
                    verts.push_back(pos + Vector3(0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3(0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3(0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3(0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3(0.5f,  0.5f,  0.5f));
                    n_verts += 6;
                }

                // Top face (towards +Y)
                if (y == Chunk_SizeY - 1 || voxels[x][y + 1][z] == Voxel_Air) {
                    verts.push_back(pos + Vector3(-0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3( 0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f,  0.5f, -0.5f));
                    n_verts += 6;
                }

                // Bottom face (towards -Y)
                if (y == 0 || voxels[x][y - 1][z] == Voxel_Air) {
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f, -0.5f, -0.5f));
                    verts.push_back(pos + Vector3( 0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3( 0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f,  0.5f));
                    verts.push_back(pos + Vector3(-0.5f, -0.5f, -0.5f));
                    n_verts += 6;
                }

                for (int i = 0; i < n_verts; ++i) {
                    mesh.colors.push_back(Voxel_ColorMap[voxel]);
                }
            }
        }
    }

    const float *vbo_data = reinterpret_cast<const float *>(verts.data());
    const float *c_data = reinterpret_cast<const float *>(mesh.colors.data());

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float) * 3, vbo_data, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, mesh.colors.size() * sizeof(float) * 4, c_data, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
}

Voxel make_voxel(const Vector3 &position) {
    float noise = snoise(position);
    if (noise > 0.3f) return Voxel_Air;
    return Voxel_Grass;
}

Chunk *load_chunk(const Point3 &position) {
    auto *chunk = new Chunk{position};
    auto world_pos = chunk->world_position();

    for (int x = 0; x < Chunk_SizeX; ++x) {
        for (int z = 0; z < Chunk_SizeZ; ++z) {

            auto pos = Vector2(world_pos.x, world_pos.z) + Vector2(Point2(x, z));
            pos /= 64;

            int height = int((snoise(pos) + 1) * 16);

            for (int y = 0; y < height/2; ++y) {
                chunk->voxels[x][y][z] = Voxel_Stone;
            }

            for (int y = height/2; y < height; ++y) {
                chunk->voxels[x][y][z] = Voxel_Grass;
            }
        }
    }
    chunk->build_chunk_mesh();

    return chunk;
}
