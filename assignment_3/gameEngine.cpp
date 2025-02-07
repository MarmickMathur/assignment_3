#include "gameEngine.h"
#include <iostream>
#include <fstream>
#include "scene_menu.h"



gameEngine::gameEngine() :
	m_running(1)
{
};

void gameEngine::init(std::string str) {
	std::ifstream myfile(str);

	if (myfile.is_open()) {
		std::string itr;
		while (myfile >> itr) {
			if (itr == "font") {
				myfile >> itr;
				std::string path = itr;
				m_assets.addfont("menuFont", path);
			}
			if (itr == "level") {
				myfile >> itr;
				std::string name = itr;
				myfile >> itr;
				std::string path = itr;
				m_assets.addLevel(name,path);
			}
			if (itr == "texture") {
				myfile >> itr;
				std::string name = itr;
				myfile >> itr;
				std::string path = itr;
				m_assets.addTexture(name, path);
			}
			if (itr == "animation") {
				std::string name , tex;
				int a, b, c, d, e, f;
				myfile >>name >> tex >> a >> b >> c >> d >> e >> f;
				m_assets.addAnimation(name,animation(name, m_assets.getTexture(tex),a,b,c,d,e,f));
			}
		}
	}
	myfile.close();

	m_window.create(sf::VideoMode(m_assets.getTexture("background").getSize().x , m_assets.getTexture("background").getSize().y), "my window" );
	m_window.setFramerateLimit(60);
	std::shared_ptr<menu> sceneMenu = std::make_shared<menu>(std::shared_ptr<gameEngine>(this));
	changeScene("Menu" , sceneMenu);
}

void gameEngine::run() {
	while (m_running) {
			update();
			sUserInput();
	}
};

void gameEngine::quit() {
	m_running = 0;
}

void gameEngine::update() {
	currentScene()->update();
};

void gameEngine::changeScene(const std::string sceneName, std::shared_ptr<scene> scene) {
	m_currentScene = sceneName;
	m_sceneMap[sceneName] = scene;
};

assets& gameEngine::getAssets() {
	return m_assets;
};

sf::RenderWindow& gameEngine::getWindow() {
	return m_window;
};

std::shared_ptr<scene> gameEngine::currentScene() {
	return m_sceneMap[m_currentScene];
};


void gameEngine::sUserInput() {
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) {
			m_running = 0;
			m_window.close();
		}
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
			if (currentScene()->getActionMap().contains(event.key.code)) {
				std::string type = (event.type == sf::Event::KeyPressed ? "Start" : "End");
				currentScene()->doAction(action(currentScene()->getActionMap()[event.key.code],type));
			}
		}
	}
};