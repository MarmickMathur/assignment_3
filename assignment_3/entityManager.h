#pragma once
#include "entity.h"
#include <vector>
#include <map>
#include <memory>
#include <string>

typedef std::map<std::string, std::vector<std::shared_ptr<entity>>> entityMap;
typedef std::vector<std::shared_ptr<entity>> entityVec;

class entityManager {
	entityMap m_eMap;
	entityVec m_eVec;
	entityVec m_eToAdd;
	Vec2 m_scale;
	size_t m_totalEntitiesProduced;

	void removeEntities(entityVec& m_eVec);
public:
	entityManager(const Vec2& scale = {1,1});
	entityVec& getEntities(std::string tag);
	entityVec& getEntities();
	std::shared_ptr<entity> addentity(std::string str);
	void update();
};