#pragma once
#include "scene.h"

struct PlayerConfig {

};

class play :public scene {
	std::string m_levelPath;
	std::shared_ptr<entity> m_player;
	PlayerConfig m_playerConfig;
	void init();
public:
	play(std::shared_ptr<gameEngine> ge, std::string levelPath);
	void update();
	void sDoAction(const action& actionName);
	void sRender();

	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sDebug();
};