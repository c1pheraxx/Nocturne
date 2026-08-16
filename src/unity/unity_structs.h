#pragma once
#include <cstdint>

namespace nocturne::unity {

// Unity internal types (x64)
// Offsets may vary by Unity version; these are common defaults.
// Adjust offsets for your target game version.

struct Transform {
    static constexpr uintptr_t position_offset = 0x90;   // Vector3 position
    static constexpr uintptr_t rotation_offset = 0x9C;   // Vector4 rotation (quaternion)
    static constexpr uintptr_t scale_offset    = 0xAC;   // Vector3 scale
};

struct Camera {
    static constexpr uintptr_t view_matrix_offset = 0x2E4; // Matrix4x4 (some versions)
    static constexpr uintptr_t fov_offset         = 0x40;  // float fieldOfView
    static constexpr uintptr_t near_clip_offset   = 0x44;  // float nearClipPlane
    static constexpr uintptr_t far_clip_offset    = 0x48;  // float farClipPlane
};

struct GameObject {
    static constexpr uintptr_t object_name_offset = 0x60;  // char* name
};

struct Component {
    static constexpr uintptr_t game_object_offset = 0x30; // GameObject*
    static constexpr uintptr_t transform_offset     = 0x38; // Transform*
};

} // namespace nocturne::unity
