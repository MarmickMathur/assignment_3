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
	bool debugMode;
	std::string m_levelPath;
	std::shared_ptr<entity> m_player;
	std::deque<std::shared_ptr<entity>> m_background;
	bool m_dir = 0;
	int lives = 6;
	int coins = 0;
	Vec2 m_finalPoint;
	PlayerConfig m_playerConfig;
	void init();
	void init_background();
	void loadLevel();
	Vec2 gridToPos(const Vec2& gridPos, std::shared_ptr<entity>& entity);
	void spawnPlayer();
	void spawnBullet(std::shared_ptr<entity> e);
	void pause();
	void destroy(std::shared_ptr<entity>& e , animation& anim);
	void replace(std::shared_ptr<entity>& e1 , std::shared_ptr<entity>& e2);
	void spawnCoin(std::shared_ptr<entity>& tile);
	void initHud();
public:
	play(std::shared_ptr<gameEngine> ge, std::string levelPath);
	void update();
	void sDoAction(const action& actionName);
	void sRender();

	void sHud();
	void sAi();
	void sCleanup();
	void sCamera();
	void sGravity();
	void sBackground();
	void sAnimation();
	void sMovement();
	void sEnemySpawner();
	void sCollision();
	void sDebug();
};