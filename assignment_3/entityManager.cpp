#include "entityManager.h"


entityManager::entityManager(const Vec2& scale) :
	m_totalEntitiesProduced(0),
	m_scale(scale)
{
}

entityVec& entityManager::getEntities(std::string tag) {
	return m_eMap[tag];
};

entityVec& entityManager::getEntities() {
	return m_eVec;
};

std::shared_ptr<entity> entityManager::addentity(std::string tag) {
	std::shared_ptr<entity> e = std::shared_ptr<entity>(new entity(m_totalEntitiesProduced++ , tag));
	m_eToAdd.push_back(e);
	return e;
};

void entityManager::removeEntities(entityVec& m_eVec) {
	auto ne = std::remove_if(m_eVec.begin(), m_eVec.end(),
		[](auto i) {
			return !(i->isActive());
		});

	m_eVec.erase(ne, m_eVec.end());
}



void entityManager::update() {
	for (auto e : m_eToAdd) {
		m_eMap[e->getTag()].push_back(e);
		m_eVec.push_back(e);
	}

	removeEntities(m_eVec);
	for (auto& i : m_eMap) {
		removeEntities(i.second);
	}


	m_eToAdd.clear();
};