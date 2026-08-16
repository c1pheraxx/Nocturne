#include "entity_scanner.h"
#include "../unity/unity_camera.h"
#include "../unity/unity_transform.h"
#include "../math/projection.h"

namespace nocturne::registry {

EntityScanner::EntityScanner(core::Process* process, core::Memory* memory)
    : process_(process), memory_(memory), resolver_(process, memory) {}

bool EntityScanner::init() {
    return resolver_.init();
}

void EntityScanner::scan() {
    auto entities = resolver_.resolve_entities();
    registry_.clear();
    for (const auto& e : entities) {
        registry_.add(e);
    }
}

void EntityScanner::scan_players(const std::vector<std::string>& patterns) {
    auto players = resolver_.resolve_players(patterns);
    registry_.clear();
    for (const auto& p : players) {
        registry_.add(p);
    }
}

} // namespace nocturne::registry
