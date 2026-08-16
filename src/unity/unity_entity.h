#pragma once
#include "../math/vec3.h"
#include "unity_skeleton.h"
#include <string>
#include <vector>
#include <unordered_map>

namespace nocturne::unity {

struct Entity {
    int id = 0;
    std::string name;
    math::Vec3 position;
    math::Vec3 rotation;
    float distance = 0.0f;
    bool visible = false;
    bool valid = false;
    uintptr_t game_object = 0;
    uintptr_t transform = 0;
};

struct Player : Entity {
    bool is_local = false;
    bool is_npc = false;
    std::unordered_map<std::string, Bone> skeleton;
};

} // namespace nocturne::unity
