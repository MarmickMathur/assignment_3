// entity.h

#include <iostream>
#include <tuple>

class entity {
private:
    size_t m_id;
    bool m_active;
    std::string m_tag;

    // Use a tuple to store components
    std::tuple<> m_components;

    // Constructor
    entity(size_t id, const std::string& tag);

public:

    friend class entityManager;
    // Add component
    template <typename Component, typename... Args>
    void addComponent(Args&&... args);

    // Check if component exists
    template <typename Component>
    bool hasComponent() const;

    // Get component
    template <typename Component>
    Component& getComponent();

    // Remove component
    template <typename Component>
    void removeComponent();

    // Accessors
    size_t getId() const;
    bool isActive() const;
    const std::string& getTag() const;

    // Mutators
    void setTag(const std::string& tag);
    void setActive(bool active);
    //friend 
};

