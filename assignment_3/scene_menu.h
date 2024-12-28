#pragma once
#include "scene.h"
#include <map>
#include <string>

class menu :public scene
{
	sf::Text m_menuText;
	int m_index = 0;
	void init();
	std::map<std::string,std::string>& m_levelMap;

public:
	menu(const std::shared_ptr<gameEngine> ge);
	void update();
	void sDoAction(const action& action);
	void sRender();
};