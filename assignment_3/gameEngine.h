#pragma once
#include <SFML/Graphics.hpp>
#include "assets.h"
#include "scene.h"


class gameEngine {
	assets m_assets;
	sf::RenderWindow m_window;
	bool m_running = 0;
	std::map<std::string, std::shared_ptr<scene>> m_sceneMap;
	std::string m_currentScene;
public:
	gameEngine();
	void init(std::string str);
	std::shared_ptr<scene> currentScene();
	void run();
	void update();
	void quit();
	void changeScene(const std::string sceneName, std::shared_ptr<scene> scene);
	assets& getAssets();
	sf::RenderWindow& getWindow();
	void sUserInput();
};
