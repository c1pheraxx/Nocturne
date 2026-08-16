#pragma once
#include "../core/memory.h"
#include "../math/vec3.h"
#include "../math/matrix.h"
#include "unity_structs.h"
#include <cstdint>

namespace nocturne::unity {

class CameraReader {
public:
    explicit CameraReader(core::Memory* mem) : mem_(mem) {}

    math::Vec3 position(uintptr_t camera) const;
    float fov(uintptr_t camera) const;
    float near_clip(uintptr_t camera) const;
    float far_clip(uintptr_t camera) const;
    math::Matrix4x4 view_matrix(uintptr_t camera) const;

    bool is_valid(uintptr_t camera) const { return camera != 0; }

private:
    core::Memory* mem_;
};

} // namespace nocturne::unity
