#include "unity_transform.h"
#include <cmath>

namespace nocturne::unity {

math::Vec3 TransformReader::position(uintptr_t transform) const {
    if (!transform) return math::Vec3();
    return mem_->read<math::Vec3>(transform + Transform::position_offset);
}

math::Vec3 TransformReader::rotation_euler(uintptr_t transform) const {
    if (!transform) return math::Vec3();
    // Unity stores rotation as quaternion at rotation_offset
    // Returning raw Vec3 for simplicity; proper conversion requires quat->euler
    auto raw = mem_->read<math::Vec3>(transform + Transform::rotation_offset);
    return raw;
}

math::Vec3 TransformReader::scale(uintptr_t transform) const {
    if (!transform) return math::Vec3(1, 1, 1);
    return mem_->read<math::Vec3>(transform + Transform::scale_offset);
}

math::Vec3 TransformReader::forward(uintptr_t transform) const {
    // Simplified: read rotation matrix or quaternion and compute forward
    // For now, return default forward
    return math::Vec3(0, 0, 1);
}

math::Vec3 TransformReader::right(uintptr_t transform) const {
    return math::Vec3(1, 0, 0);
}

math::Vec3 TransformReader::up(uintptr_t transform) const {
    return math::Vec3(0, 1, 0);
}

} // namespace nocturne::unity
