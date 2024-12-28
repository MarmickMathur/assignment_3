#include "scene_play.h"
#include "gameEngine.h"

void play::init() {

};
play::play(std::shared_ptr<gameEngine> ge, std::string levelPath) :
	scene(ge),
	m_levelPath(levelPath)
{
	init();
};

void play::update() {
	sRender();
};
void play::sDoAction(const action& actionName) {

};
void play::sRender() {
	m_gameEngine->getWindow().clear(sf::Color(100, 149, 237));
	m_gameEngine->getWindow().display();
	//sf::Sprite background;
	//background.setTexture(*(m_gameEngine->getAssets().getTexture("background")));
};

void play::sAnimation() {
};
void play::sMovement() {
};
void play::sEnemySpawner() {
};
void play::sCollision() {
};
void play::sDebug() {
};