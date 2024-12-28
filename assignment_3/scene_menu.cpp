#include "scene_menu.h"
#include "gameEngine.h"
#include <iostream>

menu::menu(const std::shared_ptr<gameEngine> gpe):
	scene(gpe),
	m_levelMap(m_gameEngine->getAssets().getLevelPathMap())
{
	init();
}

void menu::init() {
	registerAction(sf::Keyboard::W, "Up");
	registerAction(sf::Keyboard::S, "Down");
	registerAction(sf::Keyboard::Enter, "Select");

	m_menuText.setFont((*(m_gameEngine->getAssets().getFont("menuFont"))));
}

void menu::update() {
	sRender();
}

void menu::sDoAction(const action& action) {
	if (action.type == "Start") {
		if (action.name == "Up") {
			m_index--;
			m_index = std::max(m_index, 0);
		}else if (action.name == "Down") {
			m_index++;
			m_index = std::min(m_index, (int)m_levelMap.size() -1);
		}
		else if (action.name == "Select") {
			std::cout << "select";
		}
	}
}

void menu::sRender() {
	
	m_gameEngine->getWindow().clear(sf::Color(100, 149, 237));

	std::string list = "MENU\n";
	int cnt = 0;
	for (auto i : m_levelMap) {
		if (cnt == m_index) {
			list += "\t";
		}
		list += i.first;
		list += "\n";
		cnt++;
	}

	m_menuText.setString(list);
	m_menuText.setFillColor(sf::Color::Red);

	//calculating the origin
	float x = m_menuText.getLocalBounds().width / 2 + m_menuText.getLocalBounds().left;
	float y = m_menuText.getLocalBounds().height / 2 + m_menuText.getLocalBounds().top;
	//centering the text
	m_menuText.setOrigin(x, y);
	m_menuText.setPosition(m_gameEngine->getWindow().getSize().x/2 , m_gameEngine->getWindow().getSize().y / 2);

	m_gameEngine->getWindow().draw(m_menuText);
	m_gameEngine->getWindow().display();
}
