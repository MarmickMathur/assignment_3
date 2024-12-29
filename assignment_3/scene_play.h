#pragma once
#include "scene.h"
#include "deque"

struct PlayerConfig {
	float speed;
	float maxSpeed;
	float gravity;
	float jump;
	int x, y, cx, cy;

};

class play :public scene {
	std::string m_levelPath;
	std::shared_ptr<entity> m_player;
	std::deque<std::shared_ptr<entity>> m_background;
	PlayerConfig m_playerConfig;
	void init();
	void init_background();
	void loadLevel();
	Vec2 gridToPos(const Vec2& gridPos, std::shared_ptr<entity>& entity);
	void spawnPlayer();
public:
	play(std::shared_ptr<gameEngine> ge, std::string levelPath);
	void update();
	void sDoAction(const action& actionName);
	void sRender();

	void sBackground();
	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sDebug();
};