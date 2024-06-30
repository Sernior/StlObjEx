/**
 * @file GeometryUtils.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief Geometry utils for the FAConverter library.
 * @version 0.1
 * @date 2024-06-29
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef GEOMETRY_UTILS_HPP
#define GEOMETRY_UTILS_HPP

#include "BaseStructures.hpp"
#include <array>
#include <cmath>

namespace FAConverter {

std::array<float, 3> calculateNormal(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    std::array<float, 3> normal;
    float u[3] = {v1.x - v0.x, v1.y - v0.y, v1.z - v0.z};
    float v[3] = {v2.x - v0.x, v2.y - v0.y, v2.z - v0.z};

    normal[0] = u[1] * v[2] - u[2] * v[1];
    normal[1] = u[2] * v[0] - u[0] * v[2];
    normal[2] = u[0] * v[1] - u[1] * v[0];

    // Normalize
    float length = std::sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }

    return normal;
}

Vertex normalize(const Vertex& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return {v.x / length, v.y / length, v.z / length};
}

bool rayIntersectsTriangle(const Vertex& ray_origin, const Vertex& ray_vector, const Vertex& a, const Vertex& b, const Vertex& c) {
    constexpr float epsilon = std::numeric_limits<float>::epsilon();

    Vertex edge1 = b - a;
    Vertex edge2 = c - a;
    Vertex h = ray_vector.crossProduct(edge2);
    float det = edge1.dotProduct(h);

    if (det > -epsilon && det < epsilon)
        return false;

    float inv_det = 1.0 / det;
    Vertex s = ray_origin - a;
    float u = inv_det * s.dotProduct(h);

    if (u < 0.0 || u > 1.0)
        return false;

    Vertex q = s.crossProduct(edge1);
    float v = inv_det * ray_vector.dotProduct(q);

    if (v < 0.0 || u + v > 1.0)
        return false;

    float t = inv_det * edge2.dotProduct(q);

    return t > epsilon;
}

float triangleArea(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    Vertex edge1 = v1 - v0;
    Vertex edge2 = v2 - v0;
    Vertex crossProduct = edge1.crossProduct(edge2);
    float area = 0.5f * std::sqrt(crossProduct.dotProduct(crossProduct));
    return area;
}

/*
    Face elements use surface normals to indicate their orientation. If
    vertices are ordered counterclockwise around the face, both the
    face and the normal will point toward the viewer. If the vertex
    ordering is clockwise, both will point away from the viewer. If
    vertex normals are assigned, they should point in the general
    direction of the surface normal, otherwise unpredictable results
    may occur.

    if my understanding is correct this should apply:
    https://stackoverflow.com/questions/1406029/how-to-calculate-the-volume-of-a-3d-mesh-object-the-surface-of-which-is-made-up
*/
float tetrahedronVolume(const Vertex& v0, const Vertex& v1, const Vertex& v2) {
    return v0.dotProduct(v1.crossProduct(v2)) / 6.0f;
}

} // namespace FAConverter

#endif // GEOMETRY_UTILS_HPP