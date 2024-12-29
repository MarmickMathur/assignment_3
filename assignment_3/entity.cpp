#include "entity.h"

// Constructor
entity::entity(size_t id, const std::string& tag)
    : m_id(id), m_active(true), m_tag(tag), m_components() {
}

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
