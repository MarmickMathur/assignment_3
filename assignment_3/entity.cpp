// entity.cpp
#include "entity.h"

// Constructor
entity::entity(size_t id, const std::string& tag)
    : m_id(id), m_active(true), m_tag(tag) {
};


// Accessors
size_t entity::getId() const {
    return m_id;
}

bool entity::isActive() const {
    return m_active;
}

const std::string& entity::getTag() const {
    return m_tag;
}

// Mutators
void entity::setTag(const std::string& tag) {
    m_tag = tag;
}

void entity::setActive(bool active) {
    m_active = active;
}

// Template Definitions (in .cpp)

// Add component
template <typename Component, typename... Args>
void entity::addComponent(Args&&... args) {
    static_assert(std::is_constructible<Component, Args...>::value, "Component cannot be constructed with given arguments");
    m_components = std::tuple_cat(m_components, std::make_tuple(Component(std::forward<Args>(args)...)));
}

// Check if component exists
template <typename Component>
bool entity::hasComponent() const {
    return std::apply([](auto&&... components) {
        return ((std::is_same_v<std::decay_t<decltype(components)>, Component>) || ...);
        }, m_components);
}

// Get component
template <typename Component>
Component& entity::getComponent() {
    return std::get<Component>(m_components);
}

// Remove component
template <typename Component>
void entity::removeComponent() {
    static_assert(false, "Removing component is not supported for tuple-based storage.");
}
