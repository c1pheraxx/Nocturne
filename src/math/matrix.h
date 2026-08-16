#pragma once
#include "vec3.h"

namespace nocturne::math {

struct Matrix4x4 {
    float m[4][4];

    Vec3 multiply_point(const Vec3& p) const {
        Vec4 v(
            m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3],
            m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3],
            m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3],
            m[3][0] * p.x + m[3][1] * p.y + m[3][2] * p.z + m[3][3]
        );
        if (v.w != 0.0f) {
            return Vec3(v.x / v.w, v.y / v.w, v.z / v.w);
        }
        return Vec3(v.x, v.y, v.z);
    }

    Vec3 multiply_vector(const Vec3& v) const {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }
};

} // namespace nocturne::math
