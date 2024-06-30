/**
 * @file Matrix4x4.hpp
 * @author F. Abrignani (federignoli@hotmail.it)
 * @brief Matrix 4x4 for the FAConverter library.
 * @version 0.1
 * @date 2024-06-30
 * @private
 * @copyright Copyright (c) 2024 Federico Abrignani (federignoli@hotmail.it).
 * 
 */

#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include "BaseStructures.hpp"
#include <array>
#include <cmath>

namespace FAConverter {

class Matrix4x4 {
public:
    Matrix4x4() {
        for (auto& row : _m) {
            row.fill(0.0f);
        }
        _m[3][3] = 1.0f;
    }

    static Matrix4x4 identity() {
        Matrix4x4 matrix;
        for (int i = 0; i < 4; ++i) {
            matrix._m[i][i] = 1.0f;
        }
        return matrix;
    }

    static Matrix4x4 translation(float tx, float ty, float tz) {
        Matrix4x4 matrix = identity();
        matrix._m[0][3] = tx;
        matrix._m[1][3] = ty;
        matrix._m[2][3] = tz;
        return matrix;
    }

    static Matrix4x4 scaling(float sx, float sy, float sz) {
        Matrix4x4 matrix = identity();
        matrix._m[0][0] = sx;
        matrix._m[1][1] = sy;
        matrix._m[2][2] = sz;
        return matrix;
    }

    static Matrix4x4 rotationX(float angle) {
        Matrix4x4 matrix = identity();
        float rad = angle * M_PI / 180.0f;
        matrix._m[1][1] = std::cos(rad);
        matrix._m[1][2] = -std::sin(rad);
        matrix._m[2][1] = std::sin(rad);
        matrix._m[2][2] = std::cos(rad);
        return matrix;
    }

    static Matrix4x4 rotationY(float angle) {
        Matrix4x4 matrix = identity();
        float rad = angle * M_PI / 180.0f;
        matrix._m[0][0] = std::cos(rad);
        matrix._m[0][2] = std::sin(rad);
        matrix._m[2][0] = -std::sin(rad);
        matrix._m[2][2] = std::cos(rad);
        return matrix;
    }

    static Matrix4x4 rotationZ(float angle) {
        Matrix4x4 matrix = identity();
        float rad = angle * M_PI / 180.0f;
        matrix._m[0][0] = std::cos(rad);
        matrix._m[0][1] = -std::sin(rad);
        matrix._m[1][0] = std::sin(rad);
        matrix._m[1][1] = std::cos(rad);
        return matrix;
    }

    Matrix4x4 operator*(const Matrix4x4& other) const {
        Matrix4x4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result._m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result._m[i][j] += _m[i][k] * other._m[k][j];
                }
            }
        }
        return result;
    }

    Vertex operator*(const Vertex& vertex) const {
        Vertex result;
        result.x = _m[0][0] * vertex.x + _m[0][1] * vertex.y + _m[0][2] * vertex.z + _m[0][3] * vertex.w;
        result.y = _m[1][0] * vertex.x + _m[1][1] * vertex.y + _m[1][2] * vertex.z + _m[1][3] * vertex.w;
        result.z = _m[2][0] * vertex.x + _m[2][1] * vertex.y + _m[2][2] * vertex.z + _m[2][3] * vertex.w;
        result.w = _m[3][0] * vertex.x + _m[3][1] * vertex.y + _m[3][2] * vertex.z + _m[3][3] * vertex.w;
        return result;
    }

private:
    std::array<std::array<float, 4>, 4> _m;
};

} // namespace FAConverter

#endif // MATRIX4X4_HPP