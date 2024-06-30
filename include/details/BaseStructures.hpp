/**
 * @file BaseStructures.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief Base structures for the model.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef BASE_STRUCTURES_HPP
#define BASE_STRUCTURES_HPP

#include <vector>
#include <array>

namespace FAConverter {

struct Vertex {
    float x, y, z;
    float w = 1.0f;

    Vertex operator-(const Vertex& other) const {
        return {x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Vertex crossProduct(const Vertex& v) const {
        return {
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    float dotProduct(const Vertex& v) const {
        return x * v.x + y * v.y + z * v.z;
    }
};

struct TextureVertex {
    float u, v;
    float w = 0.0f;
};

struct VertexNormal {
    float i, j, k;
};

struct FaceVertexIndex {
    int vertexIndex;
    int textureVertexIndex{};
    int normalIndex{};
};

struct Face {
    std::vector<FaceVertexIndex> vertices; // Each face can have multiple vertices, each with indices for v, vt, and vn
};

} // namespace FAConverter

#endif // BASE_STRUCTURES_HPP
