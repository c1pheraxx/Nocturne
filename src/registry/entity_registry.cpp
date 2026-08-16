#include <algorithm>
#include "entity_registry.h"

namespace nocturne::registry {

void EntityRegistry::add(const unity::Entity& entity) {
    entities_.push_back(entity);
}

void EntityRegistry::add(const unity::Player& player) {
    players_.push_back(player);
    entities_.push_back(player);
}

void EntityRegistry::remove(int id) {
    entities_.erase(
        std::remove_if(entities_.begin(), entities_.end(),
            [id](const unity::Entity& e) { return e.id == id; }),
        entities_.end());

    players_.erase(
        std::remove_if(players_.begin(), players_.end(),
            [id](const unity::Player& p) { return p.id == id; }),
        players_.end());
}

void EntityRegistry::clear() {
    entities_.clear();
    players_.clear();
}

unity::Entity* EntityRegistry::find_entity(int id) {
    for (auto& e : entities_) {
        if (e.id == id) return &e;
    }
    return nullptr;
}

unity::Player* EntityRegistry::find_player(int id) {
    for (auto& p : players_) {
        if (p.id == id) return &p;
    }
    return nullptr;
}

unity::Player* EntityRegistry::find_player(const std::string& name) {
    for (auto& p : players_) {
        if (p.name == name) return &p;
    }
    return nullptr;
}

void EntityRegistry::update_distances(const math::Vec3& reference) {
    for (auto& e : entities_) {
        e.distance = e.position.distance(reference);
    }
    for (auto& p : players_) {
        p.distance = p.position.distance(reference);
    }
}

void EntityRegistry::cleanup() {
    entities_.erase(
        std::remove_if(entities_.begin(), entities_.end(),
            [](const unity::Entity& e) { return !e.valid; }),
        entities_.end());

    players_.erase(
        std::remove_if(players_.begin(), players_.end(),
            [](const unity::Player& p) { return !p.valid; }),
        players_.end());
}

} // namespace nocturne::registry
