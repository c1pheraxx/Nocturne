#include "unity_resolver.h"
#include "unity_structs.h"
#include "unity_transform.h"
#include "unity_camera.h"
#include "unity_skeleton.h"
#include <algorithm>

namespace nocturne::unity {

UnityResolver::UnityResolver(core::Process* process, core::Memory* memory)
    : process_(process), memory_(memory) {}

bool UnityResolver::init() {
    if (!resolve_game_object_manager()) return false;
    if (!resolve_camera_base()) return false;
    return true;
}

bool UnityResolver::resolve_game_object_manager() {
    // Pattern for GameObjectManager (Unity 2020-2022 x64)
    // This pattern is illustrative; real patterns require dumping the target
    auto result = core::Pattern::scan_module(process_, L"UnityPlayer.dll",
        "48 8B 05 ?? ?? ?? ?? 48 8B 48 08 48 8B 01 FF 50 ?? 48 85 C0");
    if (!result.found) return false;

    // Resolve RIP-relative address
    int32_t rel = memory_->read<int32_t>(result.address + 3);
    game_object_manager_ = result.address + 7 + rel;
    return game_object_manager_ != 0;
}

bool UnityResolver::resolve_camera_base() {
    // Pattern for Camera.main or Camera.current
    auto result = core::Pattern::scan_module(process_, L"UnityPlayer.dll",
        "48 8B 05 ?? ?? ?? ?? 48 8B 88 ?? ?? ?? ?? 48 8B 01 FF 50 ?? 48 85 C0");
    if (!result.found) return false;

    int32_t rel = memory_->read<int32_t>(result.address + 3);
    camera_base_ = result.address + 7 + rel;
    return camera_base_ != 0;
}

uintptr_t UnityResolver::resolve_camera() const {
    if (!camera_base_) return 0;
    return memory_->read<uintptr_t>(camera_base_);
}

std::vector<Entity> UnityResolver::resolve_entities() const {
    std::vector<Entity> entities;
    if (!game_object_manager_) return entities;

    // GOM structure (simplified):
    // GOM -> [0x8] -> TaggedObjects -> [0x10] ActiveObjects -> ...
    // This is highly version-dependent. Adjust for your target.
    uintptr_t tagged = memory_->read<uintptr_t>(game_object_manager_ + 0x8);
    if (!tagged) return entities;

    uintptr_t active = memory_->read<uintptr_t>(tagged + 0x10);
    if (!active) return entities;

    // Walk the linked list (simplified)
    // Real implementation needs proper list traversal with next/prev
    for (int i = 0; i < 1000; ++i) {
        uintptr_t obj = memory_->read<uintptr_t>(active + 0x28 + i * 0x8);
        if (!obj) continue;

        Entity e;
        e.game_object = obj;
        e.transform = memory_->read<uintptr_t>(obj + Component::transform_offset);

        TransformReader tr(memory_);
        e.position = tr.position(e.transform);
        e.valid = true;

        uintptr_t name_ptr = memory_->read<uintptr_t>(obj + GameObject::object_name_offset);
        if (name_ptr) e.name = memory_->read_string(name_ptr);

        entities.push_back(e);
    }

    return entities;
}

std::vector<Player> UnityResolver::resolve_players(const std::vector<std::string>& namePatterns) const {
    auto entities = resolve_entities();
    std::vector<Player> players;

    for (const auto& e : entities) {
        for (const auto& pat : namePatterns) {
            if (e.name.find(pat) != std::string::npos) {
                Player p;
                static_cast<Entity&>(p) = e;
                p.is_npc = (e.name.find("NPC") != std::string::npos || e.name.find("Bot") != std::string::npos);

                SkeletonReader sr(memory_);
                p.skeleton = sr.read_skeleton(e.transform);
                players.push_back(p);
                break;
            }
        }
    }

    return players;
}

Player UnityResolver::resolve_local_player() const {
    auto players = resolve_players({"Player", "Local", "Hero", "Character"});
    for (auto& p : players) {
        if (p.name.find("Local") != std::string::npos || p.name.find("Player") != std::string::npos) {
            p.is_local = true;
            return p;
        }
    }
    return Player{};
}

} // namespace nocturne::unity
