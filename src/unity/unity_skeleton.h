#pragma once
#include "../core/memory.h"
#include "../math/vec3.h"
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>

namespace nocturne::unity {

struct Bone {
    std::string name;
    math::Vec3 position;
    math::Vec3 rotation;
    bool valid = false;
};

class SkeletonReader {
public:
    explicit SkeletonReader(core::Memory* mem) : mem_(mem) {}

    // Reads bones from a SkinnedMeshRenderer or Animator
    // root: address of the root Transform
    // Returns map of bone name -> Bone
    std::unordered_map<std::string, Bone> read_skeleton(uintptr_t rootTransform) const;

    // Common bone aliases
    static const char* alias_head() { return "Head"; }
    static const char* alias_neck() { return "Neck"; }
    static const char* alias_chest() { return "Chest"; }
    static const char* alias_spine() { return "Spine"; }
    static const char* alias_hips() { return "Hips"; }
    static const char* alias_left_hand() { return "LeftHand"; }
    static const char* alias_right_hand() { return "RightHand"; }
    static const char* alias_left_foot() { return "LeftFoot"; }
    static const char* alias_right_foot() { return "RightFoot"; }

private:
    core::Memory* mem_;

    void enumerate_children(uintptr_t transform, std::vector<uintptr_t>& out) const;
    std::string read_transform_name(uintptr_t transform) const;
};

} // namespace nocturne::unity
