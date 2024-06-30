/**
 * @file ModelOBJ.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief OBJ model implementation.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef MODEL_OBJ_HPP
#define MODEL_OBJ_HPP

#include "FileIOTypes.hpp"
#include "BaseStructures.hpp"
#include "GeometryUtils.hpp"
#include "Matrix4x4.hpp"
#include <string>
#include <fstream>
#include <cmath>
#include <limits>

namespace FAConverter {

// Specialization for OBJ files
template<>
class Model<FileType::OBJ> {

public:

    Model() = default;
    void read(const std::string& filename);
    template<FileType U>
    void write(const std::string& filename) const;
    void applyTransform(const Matrix4x4& transform);
    bool isPointInside(const Vertex& point) const;
    float calculateSurfaceArea() const;
    float calculateVolume() const;

private:

    std::vector<Vertex> vertices;
    std::vector<TextureVertex> textureVertices;
    std::vector<VertexNormal> vertexNormals;
    std::vector<Face> faces;

};

// Implementation for reading OBJ files
void Model<FileType::OBJ>::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vertex vertex;
            iss >> vertex.x >> vertex.y >> vertex.z >> vertex.w;
            vertices.push_back(vertex);
        } else if (prefix == "vt") {
            TextureVertex textureVertex;
            iss >> textureVertex.u >> textureVertex.v >> textureVertex.w;
            textureVertices.push_back(textureVertex);
        } else if (prefix == "vn") {
            VertexNormal vertexNormal;
            iss >> vertexNormal.i >> vertexNormal.j >> vertexNormal.k;
            vertexNormals.push_back(vertexNormal);
        } else if (prefix == "f") {
            Face face;
            std::string vertex;
            while (iss >> vertex) {
                FaceVertexIndex faceVertex;
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream viss(vertex);
                viss >> faceVertex.vertexIndex >> faceVertex.textureVertexIndex >> faceVertex.normalIndex;
                
                /*
                In addition to counting vertices down from the top of the first list in
                the file, you can also count vertices back up the list from an
                element's position in the file. When you count up the list from an
                element, the reference numbers are negative. A reference number of -1
                indicates the vertex immediately above the element. A reference number
                of -2 indicates two references above and so on.*/

                if (faceVertex.vertexIndex < 0) {
                    faceVertex.vertexIndex = vertices.size() - faceVertex.vertexIndex;
                }
                if (faceVertex.textureVertexIndex < 0) {
                    faceVertex.textureVertexIndex = textureVertices.size() - faceVertex.textureVertexIndex;
                }
                if (faceVertex.normalIndex < 0) {
                    faceVertex.normalIndex = vertexNormals.size() - faceVertex.normalIndex;
                }

                face.vertices.push_back(faceVertex);
            }
            faces.push_back(face);
        }
    }

    file.close();
}

template<>
void Model<FileType::OBJ>::write<FileType::STL>(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing");
    }

    // Write 80-byte header... maybe should set these bytes to zero just to be sure but should be fine
    char header[80] = {};
    file.write(header, 80);

    uint32_t numTriangles = 0;
    for (const auto& face : faces) {
        numTriangles += face.vertices.size() - 2;
    }
    file.write(reinterpret_cast<const char*>(&numTriangles), sizeof(numTriangles));

    // Write each triangle
    for (const auto& face : faces) {
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {

            const Vertex& v0 = vertices[face.vertices[0].vertexIndex - 1];
            const Vertex& v1 = vertices[face.vertices[i].vertexIndex - 1];
            const Vertex& v2 = vertices[face.vertices[i + 1].vertexIndex - 1];
            
            // Calculate or use provided normal
            std::array<float, 3> normal;
            if (face.vertices[0].normalIndex > 0) {
                const VertexNormal& vn = vertexNormals[face.vertices[0].normalIndex - 1];
                normal = {vn.i, vn.j, vn.k};
            } else {
                normal = calculateNormal(v0, v1, v2);
            }
            file.write(reinterpret_cast<const char*>(normal.data()), sizeof(float) * 3);

            file.write(reinterpret_cast<const char*>(&v0.x), sizeof(v0.x) * 3);
            file.write(reinterpret_cast<const char*>(&v1.x), sizeof(v1.x) * 3);
            file.write(reinterpret_cast<const char*>(&v2.x), sizeof(v2.x) * 3);

            // Write attribute byte count we could set a color here for certain softwares
            uint16_t attributeByteCount = 0;
            file.write(reinterpret_cast<const char*>(&attributeByteCount), sizeof(attributeByteCount));
        }
    }

    file.close();
}

void Model<FileType::OBJ>::applyTransform(const Matrix4x4& transform) {
    for (auto& vertex : vertices) {
        vertex = transform * vertex;
    }
}

/*
https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
We use the Mller–Trumbore intersection algorithm with some optimizations...
First since we are going towards the positive x we can ignore all triangles that have a vertex with x coordinate smaller than the point x.
Second if a triangle intersects the ray we can ignore all other triangles of the current face.
*/
bool Model<FileType::OBJ>::isPointInside(const Vertex& point) const {
    int intersections = 0;
    Vertex ray_vector = {1.0f, 0.0f, 0.0f, 0.0f};

    for (const auto& face : faces) {
        bool faceIntersected = false;
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {
            const Vertex& v0 = vertices[face.vertices[0].vertexIndex - 1];
            const Vertex& v1 = vertices[face.vertices[i].vertexIndex - 1];
            const Vertex& v2 = vertices[face.vertices[i + 1].vertexIndex - 1];

            float maxX = std::max({v0.x, v1.x, v2.x});

            if (maxX < point.x) {
                continue;
            }

            if (rayIntersectsTriangle(point, ray_vector, v0, v1, v2)) {
                intersections++;
                faceIntersected = true;
                break;
            }
        }
        if (faceIntersected) {
            continue;
        }
    }

    return (intersections % 2) == 1;
}

float Model<FileType::OBJ>::calculateSurfaceArea() const {
    float totalArea = 0.0f;

    for (const auto& face : faces) {
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {
            const Vertex& v0 = vertices[face.vertices[0].vertexIndex - 1];
            const Vertex& v1 = vertices[face.vertices[i].vertexIndex - 1];
            const Vertex& v2 = vertices[face.vertices[i + 1].vertexIndex - 1];

            totalArea += triangleArea(v0, v1, v2);
        }
    }

    return totalArea;
}

float Model<FileType::OBJ>::calculateVolume() const {
    /*float totalVolume = 0.0f;

    for (const auto& face : faces) {
        for (size_t i = 1; i < face.vertices.size() - 1; ++i) {
            const Vertex& v0 = vertices[face.vertices[0].vertexIndex - 1];
            const Vertex& v1 = vertices[face.vertices[i].vertexIndex - 1];
            const Vertex& v2 = vertices[face.vertices[i + 1].vertexIndex - 1];

            totalVolume += tetrahedronVolume(v0, v1, v2);
        }
    }

    return std::abs(totalVolume);*/
    // does not seem to work on my examples need to think about this more
    // TODO
    return 0.0f;
}

} // namespace FAConverter

#endif // MODEL_OBJ_HPP