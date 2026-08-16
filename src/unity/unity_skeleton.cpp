#include "unity_skeleton.h"
#include "unity_structs.h"
#include <cstring>

namespace nocturne::unity {

std::unordered_map<std::string, Bone> SkeletonReader::read_skeleton(uintptr_t rootTransform) const {
    std::unordered_map<std::string, Bone> result;
    if (!rootTransform) return result;

    std::vector<uintptr_t> transforms;
    enumerate_children(rootTransform, transforms);

    for (uintptr_t t : transforms) {
        std::string name = read_transform_name(t);
        if (name.empty()) continue;

        Bone b;
        b.name = name;
        b.position = mem_->read<math::Vec3>(t + Transform::position_offset);
        b.rotation = mem_->read<math::Vec3>(t + Transform::rotation_offset);
        b.valid = true;
        result[name] = b;
    }

    return result;
}

void SkeletonReader::enumerate_children(uintptr_t transform, std::vector<uintptr_t>& out) const {
    if (!transform) return;
    out.push_back(transform);

    // Unity Transform children: m_Children array
    // Offset varies; common: transform + 0x40 -> List<Transform*>
    // This is a simplified traversal; adjust offsets for your target
    uintptr_t child_array = mem_->read<uintptr_t>(transform + 0x40);
    if (!child_array) return;

    int child_count = mem_->read<int>(child_array + 0x18);
    if (child_count <= 0 || child_count > 100) return;

    for (int i = 0; i < child_count; ++i) {
        uintptr_t child = mem_->read<uintptr_t>(child_array + 0x20 + i * 0x8);
        if (child) enumerate_children(child, out);
    }
}

std::string SkeletonReader::read_transform_name(uintptr_t transform) const {
    if (!transform) return "";
    // Transform -> GameObject -> m_Name
    uintptr_t game_object = mem_->read<uintptr_t>(transform + 0x30);
    if (!game_object) return "";

    uintptr_t name_ptr = mem_->read<uintptr_t>(game_object + GameObject::object_name_offset);
    if (!name_ptr) return "";

    return mem_->read_string(name_ptr);
}

} // namespace nocturne::unity
