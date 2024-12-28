#pragma once
#include "entityManager.h"
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <memory>
#include "action.h"
typedef std::map<int, std::string> ActionMap;

class gameEngine;

class scene {
protected:
	std::shared_ptr<gameEngine> m_gameEngine;
	entityManager m_entities;
	size_t currentFrame;
	ActionMap m_actionMap;
	bool paused;
	bool hasEnded;
public:
	scene(const std::shared_ptr<gameEngine> m_ge);
	virtual void update() = 0;
	virtual void sDoAction(const action& action) = 0;
	virtual void sRender() = 0;

	ActionMap getActionMap();
	void simulate();
	void doAction(const action& action);
	void registerAction(int inputCode, const std::string actionName);
};
