#pragma once
#include "../core/process.h"
#include "../core/memory.h"
#include "../unity/unity_resolver.h"
#include "entity_registry.h"

namespace nocturne::registry {

class EntityScanner {
public:
    EntityScanner(core::Process* process, core::Memory* memory);

    bool init();
    void scan();
    void scan_players(const std::vector<std::string>& patterns);

    EntityRegistry& registry() { return registry_; }
    const EntityRegistry& registry() const { return registry_; }

private:
    core::Process* process_;
    core::Memory* memory_;
    unity::UnityResolver resolver_;
    EntityRegistry registry_;
};

} // namespace nocturne::registry
