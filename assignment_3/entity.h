#pragma once
#include <string>
#include <tuple>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include "components.h" // Include your component headers

// Define a tuple to hold all possible components
typedef std::tuple<
    cTransform,
    cBoundingBox,
    cLifeSpan,
    cInput,
    cAnimation,
    cState,
    cGravity
>  ComponentTuple;

class entity {
private:
    size_t m_id;
    bool m_active;
    std::string m_tag;

    // Private constructor, can only be created by entityManager
    entity(size_t id, const std::string& tag);

public:
    ComponentTuple m_components;
    friend class entityManager;

    // Add component to the entity
    template <typename Component, typename... Args>
    void addComponent(Args&&... args) {
        std::get<Component>(m_components) = Component(std::forward<Args>(args)...);
        std::get<Component>(m_components).has = true;
    }

    // Check if the component exists (default-initialized check)
    template <typename Component>
    bool hasComponent() const {
        return std::get<Component>(m_components).has;
    }

    // Get component (throws if component doesn't exist)
    template <typename Component>
    Component& getComponent() {
        if (!hasComponent<Component>()) {
            throw std::runtime_error("Component does not exist.");
        }
        return std::get<Component>(m_components);
    }

    // Remove component by resetting it to default
    template <typename Component>
    void removeComponent() {
        std::get<Component>(m_components) = Component();
        std::get<Component>(m_components).has = 0;
    }

    // Accessors
    size_t getId() const;
    bool isActive() const;
    const std::string& getTag() const;

    // Mutators
    void setTag(const std::string& tag);
    void setActive(bool active);
};
