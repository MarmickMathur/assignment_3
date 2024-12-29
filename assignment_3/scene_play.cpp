#include "scene_play.h"
#include "gameEngine.h"
#include <iostream>
#include <fstream>

void play::init_background() {

	for (int i = 0; i < 3; i++) {
		m_background.push_back(m_entities.addentity("dec"));
		animation background("background", m_gameEngine->getAssets().getTexture("background"));
		m_background.back()->addComponent<cAnimation>(background, 1);
		sf::Vector2u windowSize = m_gameEngine->getWindow().getSize();             // Get size of the window
		sf::Vector2f viewcenter = m_gameEngine->getWindow().getView().getCenter();
		float mult = (i % 2 == 0 ? -1 : 1);
		m_background.back()->getComponent<cAnimation>().anim.getSprite().setScale(mult ,1); // Scale to fit the window
		m_background.back()->getComponent<cAnimation>().anim.getSprite().setPosition(viewcenter.x + ((float)i - 1) * (windowSize.x / 2.0), (windowSize.y / 2.0));
		//std::cout << viewcenter.x + ((float)i - 1) * (windowSize.x / 2.0) << " " << (windowSize.y / 2.0) << "\n";
	}
}

Vec2 play::gridToPos(const Vec2& gridPos, std::shared_ptr<entity>& entity) {
	float height = m_gameEngine->getWindow().getSize().y / 16;
	return Vec2(gridPos.x * 16 + entity->getComponent<cAnimation>().anim.getSize().x / 2, (height - gridPos.y - 1) * 16 + entity->getComponent<cAnimation>().anim.getSize().y / 2);
}

void play::loadLevel() {
	m_entities = entityManager();
	init_background();


	std::ifstream levelFile(m_levelPath);
	std::string itr;
	if (levelFile.is_open()) {
		while (levelFile >> itr) {
			auto e = m_entities.addentity(itr);
			if (itr == "player") {
				levelFile >> m_playerConfig.speed >> m_playerConfig.maxSpeed >> m_playerConfig.gravity >> m_playerConfig.jump;
				levelFile >> m_playerConfig.x >> m_playerConfig.y >> m_playerConfig.cx >> m_playerConfig.cy;
				continue;
			}
			levelFile >> itr;
			e->addComponent<cAnimation>(animation(itr , m_gameEngine->getAssets().getTexture(itr)), 1);
			levelFile >> itr;
			int x = std::stoi(itr);
			levelFile >> itr;
			int y = std::stoi(itr);
			e->addComponent<cTransform>(gridToPos(Vec2(x, y), e), Vec2(0, 0));
		}
	}

	spawnPlayer();
}



void play::spawnPlayer() {
	m_player = m_entities.addentity("player");
	m_player->addComponent<cInput>();
	//m_gameEngine->getAssets().getAnimation("standing").update();
	m_player->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("standing") , 1);
	m_player->addComponent<cTransform>(gridToPos(Vec2(m_playerConfig.x, m_playerConfig.y) , m_player), Vec2(0, 0));
	m_player->addComponent<cGravity>(m_playerConfig.gravity);
	m_player->addComponent<cBoundingBox>(Vec2(m_playerConfig.cx, m_playerConfig.cy));
	m_player->addComponent<cState>();
}

void play::init() {

	loadLevel();
	registerAction(sf::Keyboard::W, "Jump");
	registerAction(sf::Keyboard::S, "Duck");
	registerAction(sf::Keyboard::D, "MoveRight");
	registerAction(sf::Keyboard::A, "MoveLeft");
	registerAction(sf::Keyboard::Space, "Shoot");

};

play::play(std::shared_ptr<gameEngine> ge, std::string levelPath) :
	scene(ge),
	m_levelPath(levelPath)
{
	init();
};



void play::update() {
	m_entities.update();
	sBackground();
	sAnimation();
	sRender();
};

void play::sDoAction(const action& actionName) {
	if (actionName.type == "Start") {
		if (actionName.name == "Jump") { m_player->getComponent<cInput>().up = 1; }
		if (actionName.name == "Duck") { m_player->getComponent<cInput>().down = 1; }
		if (actionName.name == "MoveRight") { m_player->getComponent<cInput>().right = 1; }
		if (actionName.name == "MoveLeft") { m_player->getComponent<cInput>().left = 1; }
	}else{
		if (actionName.name == "Jump") { m_player->getComponent<cInput>().up = 0; }
		if (actionName.name == "Duck") { m_player->getComponent<cInput>().down = 0; }
		if (actionName.name == "MoveRight") { m_player->getComponent<cInput>().right = 0; }
		if (actionName.name == "MoveLeft") { m_player->getComponent<cInput>().left = 0; }
	}
};

void play::sBackground() {
	sf::Vector2f viewcenter = m_gameEngine->getWindow().getView().getCenter();
	if (viewcenter.x >= m_background.back()->getComponent<cAnimation>().anim.getSprite().getPosition().x) {
		m_background.push_back(m_background.front());
		m_background.pop_front();
		m_background.back()->getComponent<cAnimation>().anim.getSprite().scale(-1.0 , 1);
	}
	if (viewcenter.x <= m_background.front()->getComponent<cAnimation>().anim.getSprite().getPosition().x) {
		m_background.push_front(m_background.back());
		m_background.pop_back();
		m_background.front()->getComponent<cAnimation>().anim.getSprite().scale(-1.0, 1);
	}
}

void play::sRender() {
	m_gameEngine->getWindow().clear(sf::Color(100, 149, 237));
	for (auto &e : m_entities.getEntities()) {
		if (e->hasComponent<cAnimation>()) {
			if (e->hasComponent<cTransform>()) {
				Vec2& pos = e->getComponent<cTransform>().pos;
				e->getComponent<cAnimation>().anim.getSprite().setPosition(pos.x , pos.y);
			}
			m_gameEngine->getWindow().draw(e->getComponent<cAnimation>().anim.getSprite());
		}
	}
	m_gameEngine->getWindow().display();
};

void play::sAnimation() {
	for (auto &e : m_entities.getEntities()) {
		if (e->hasComponent<cAnimation>()) {
			e->getComponent<cAnimation>().anim.update();
		}
	}
};

void play::sMovement() {
	cInput& inputComp = m_player->getComponent<cInput>();
	cTransform& transformComp = m_player->getComponent<cTransform>();

	if (inputComp.up) {
		transformComp.vel.y = -m_playerConfig.jump;
	}
	else if (inputComp.left) {
		transformComp.vel.x = -m_playerConfig.speed;
	}
	else if (inputComp.right) {
		transformComp.vel.x = m_playerConfig.speed;
	}
	else if (inputComp.down) {
		transformComp.vel.y = m_playerConfig.speed;
	}
};


void play::sEnemySpawner() {
};
void play::sCollision() {
};
void play::sDebug() {
};