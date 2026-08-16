#pragma once
#include "../core/memory.h"
#include "../math/vec3.h"
#include "unity_structs.h"
#include <cstdint>

namespace nocturne::unity {

class TransformReader {
public:
    explicit TransformReader(core::Memory* mem) : mem_(mem) {}

    math::Vec3 position(uintptr_t transform) const;
    math::Vec3 rotation_euler(uintptr_t transform) const; // simplified
    math::Vec3 scale(uintptr_t transform) const;

    math::Vec3 forward(uintptr_t transform) const;
    math::Vec3 right(uintptr_t transform) const;
    math::Vec3 up(uintptr_t transform) const;

private:
    core::Memory* mem_;
};

} // namespace nocturne::unity
