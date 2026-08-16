#include "unity_camera.h"

namespace nocturne::unity {

math::Vec3 CameraReader::position(uintptr_t camera) const {
    if (!camera) return math::Vec3();
    // Camera position is usually at transform->position
    // Here we read a simplified offset; adjust for your target
    return mem_->read<math::Vec3>(camera + 0x42C);
}

float CameraReader::fov(uintptr_t camera) const {
    if (!camera) return 60.0f;
    return mem_->read<float>(camera + Camera::fov_offset);
}

float CameraReader::near_clip(uintptr_t camera) const {
    if (!camera) return 0.1f;
    return mem_->read<float>(camera + Camera::near_clip_offset);
}

float CameraReader::far_clip(uintptr_t camera) const {
    if (!camera) return 1000.0f;
    return mem_->read<float>(camera + Camera::far_clip_offset);
}

math::Matrix4x4 CameraReader::view_matrix(uintptr_t camera) const {
    if (!camera) return math::Matrix4x4{};
    // View matrix in Unity is often at Camera + 0x2E4 or 0x300+ depending on version
    // This is a placeholder; use pattern scan or dump to find correct offset
    return mem_->read<math::Matrix4x4>(camera + Camera::view_matrix_offset);
}

} // namespace nocturne::unity
