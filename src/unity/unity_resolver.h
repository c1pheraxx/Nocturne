#pragma once
#include "../core/process.h"
#include "../core/memory.h"
#include "../core/pattern.h"
#include "unity_entity.h"
#include <vector>
#include <string>

namespace nocturne::unity {

class UnityResolver {
public:
    UnityResolver(core::Process* process, core::Memory* memory);

    bool init();

    // Find Camera.main
    uintptr_t resolve_camera() const;

    // Find all active GameObjects via GOM (GameObjectManager)
    std::vector<Entity> resolve_entities() const;

    // Find players by tag or name pattern
    std::vector<Player> resolve_players(const std::vector<std::string>& namePatterns) const;

    // Resolve local player (usually tagged "Player" or "LocalPlayer")
    Player resolve_local_player() const;

private:
    core::Process* process_;
    core::Memory* memory_;

    uintptr_t game_object_manager_ = 0;
    uintptr_t camera_base_ = 0;

    bool resolve_game_object_manager();
    bool resolve_camera_base();
};

} // namespace nocturne::unity
