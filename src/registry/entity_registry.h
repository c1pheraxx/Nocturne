#pragma once
#include "../unity/unity_entity.h"
#include <vector>
#include <string>

namespace nocturne::registry {

class EntityRegistry {
public:
    void add(const unity::Entity& entity);
    void add(const unity::Player& player);
    void remove(int id);
    void clear();

    const std::vector<unity::Entity>& entities() const { return entities_; }
    const std::vector<unity::Player>& players() const { return players_; }

    unity::Entity* find_entity(int id);
    unity::Player* find_player(int id);
    unity::Player* find_player(const std::string& name);

    void update_distances(const math::Vec3& reference);
    void cleanup();

    size_t entity_count() const { return entities_.size(); }
    size_t player_count() const { return players_.size(); }

private:
    std::vector<unity::Entity> entities_;
    std::vector<unity::Player> players_;
};

} // namespace nocturne::registry
