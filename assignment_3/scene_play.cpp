#include "scene_play.h"
#include "gameEngine.h"
#include <iostream>
#include <fstream>
#include "physics.h"
#include "scene_menu.h"


void play::init_background() {

	for (int i = 0; i < 3; i++) {
		m_background.push_back(m_entities.addentity("dec"));
		animation background("background", m_gameEngine->getAssets().getTexture("background"));
		m_background.back()->addComponent<cAnimation>(background, 1);
		sf::Vector2 textSize = { m_gameEngine->getAssets().getTexture("background").getSize().x , m_gameEngine->getAssets().getTexture("background").getSize().y };
		sf::Vector2f viewcenter = m_gameEngine->getWindow().getView().getCenter();
		float mult = (i % 2 == 0 ? -1 : 1);
		m_background.back()->getComponent<cAnimation>().anim.getSprite().scale(mult ,1); // Scale to fit the window
		m_background.back()->addComponent<cTransform>(Vec2(viewcenter.x + ((float)i - 1) * (textSize.x ), (textSize.y /2.0)) , Vec2(0,0));
	}
}

Vec2 play::gridToPos(const Vec2& gridPos, std::shared_ptr<entity>& entity) {
	float height = m_gameEngine->getWindow().getView().getSize().y / 16;
	return Vec2(gridPos.x * 16 + entity->getComponent<cAnimation>().anim.getSize().x / 2, (height - gridPos.y - 1) * 16 + entity->getComponent<cAnimation>().anim.getSize().y / 2);
}

void play::loadLevel() {
	m_entities = entityManager();
	init_background();


	std::ifstream levelFile(m_levelPath);
	std::string itr;
	if (levelFile.is_open()) {
		while (levelFile >> itr) {
			if (itr == "player") {
				levelFile >> m_playerConfig.speed >> m_playerConfig.maxSpeed >> m_playerConfig.gravity >> m_playerConfig.jump;
				levelFile >> m_playerConfig.x >> m_playerConfig.y >> m_playerConfig.cx >> m_playerConfig.cy;
				continue;
			}
			auto e = m_entities.addentity(itr);
			levelFile >> itr;
			e->addComponent<cAnimation>(animation(itr , m_gameEngine->getAssets().getTexture(itr)), 1);
			if (e->getTag() == "enemy") e->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("enemy1Walking") , 1);
			if (e->getTag() != "dec") e->addComponent<cBoundingBox>(e->getComponent<cAnimation>().anim.getSize());
			levelFile >> itr;

			int x = std::stoi(itr);
			levelFile >> itr;
			int y = std::stoi(itr);
			if (e->getComponent<cAnimation>().anim.getName() == "flag") {
				m_finalPoint.x = x;
				m_finalPoint.y = y;
			}
			e->addComponent<cTransform>(gridToPos(Vec2(x, y), e), Vec2(0, 0));
			if (e->getTag() == "enemy") {
				e->getComponent<cTransform>().vel.x = m_playerConfig.speed * 0.5;
				e->addComponent<cGravity>(m_playerConfig.gravity);
			}
	
		}
	}



	spawnPlayer();
	initHud();
}

void play::initHud() {
	for (int i = 0; i < lives;i++) {
		auto heart = m_entities.addentity("hudHeart");
		heart->addComponent<cTransform>(Vec2(0,0) , Vec2(0, 0));
		heart->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("heart") , 1);
	}
	auto coin = m_entities.addentity("hudCoin");
	coin->addComponent<cTransform>(Vec2(0, 0) , Vec2(0, 0));
	coin->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("coin"), 1);
}


void play::spawnPlayer() {
	if (m_entities.getEntities("player").size() != 0) return;
	lives--;
	if(m_entities.getEntities("hudHeart").size() != 0) m_entities.getEntities("hudHeart")[0]->setActive(0);
	m_player = m_entities.addentity("player");
	m_player->addComponent<cInput>();
	m_player->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("standing") , 1);
	m_player->addComponent<cTransform>(gridToPos(Vec2(m_playerConfig.x, m_playerConfig.y) , m_player), Vec2(0, 0));
	m_player->addComponent<cGravity>(m_playerConfig.gravity);
	m_player->addComponent<cBoundingBox>(Vec2(m_playerConfig.cx, m_playerConfig.cy));
	m_player->addComponent<cState>();
}

void play::pause() {
	paused = 1;
}

void play::spawnCoin(std::shared_ptr<entity>& tile) {

	std::shared_ptr<entity> coin = m_entities.addentity("coin");
	coin->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("coinSpin"), 1);
	coin->addComponent<cBoundingBox>(coin->getComponent<cAnimation>().anim.getSize());
	coin->addComponent<cLifeSpan>(200);
	coin->addComponent<cTransform>(Vec2(tile->getComponent<cTransform>().pos.x  ,tile->getComponent<cTransform>().pos.y - coin->getComponent<cBoundingBox>().dimensions.y/2.0 - tile->getComponent<cBoundingBox>().dimensions.y / 2.0) , Vec2(0,0));
}

void play::replace(std::shared_ptr<entity>& e1 , std::shared_ptr<entity>& e2) {
	e2->addComponent<cTransform>(e1->getComponent<cTransform>());
	e2->addComponent<cAnimation>(e1->getComponent<cAnimation>());
	e2->addComponent<cBoundingBox>(e1->getComponent<cBoundingBox>().dimensions);
	e1->setActive(0);
}

void play::destroy(std::shared_ptr<entity>& e , animation& anim) {
	e->addComponent<cAnimation>(anim , 0);
	e->getComponent<cTransform>().vel = {0,0};
	e->removeComponent<cGravity>();
	e->removeComponent<cBoundingBox>();
}

void play::spawnBullet(std::shared_ptr<entity> pistol) {
	std::string bulletName = "enemyBullet";
	if (pistol == m_player) bulletName = "playerBullet";
	auto e = m_entities.addentity(bulletName);
	e->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation("bullet") , 1);
	e->addComponent<cBoundingBox>(e->getComponent<cAnimation>().anim.getSize());
	float posx = pistol->getComponent<cTransform>().pos.x + pistol->getComponent<cBoundingBox>().dimensions.x / 2;
	float posy = pistol->getComponent<cTransform>().pos.y;
	int dir = pistol->getComponent<cAnimation>().anim.m_dir;
	e->addComponent<cTransform>(Vec2(posx, posy), Vec2(dir * m_playerConfig.speed * 2, 0));
}

void play::init() {
	loadLevel();
	registerAction(sf::Keyboard::W, "Jump");
	registerAction(sf::Keyboard::S, "Duck");
	registerAction(sf::Keyboard::D, "MoveRight");
	registerAction(sf::Keyboard::A, "MoveLeft");
	registerAction(sf::Keyboard::Space, "Shoot");
	registerAction(sf::Keyboard::T, "Debug");
	registerAction(sf::Keyboard::P, "Pause");
	registerAction(sf::Keyboard::Escape, "Quit");

};

play::play(std::shared_ptr<gameEngine> ge, std::string levelPath) :
	scene(ge),
	debugMode(0),
	m_levelPath(levelPath)
{
	init();
};


void play::update() {
	if (lives <= 0) {
		hasEnded = 1;
	}
	if (hasEnded) {
		std::shared_ptr<menu> sceneMenu = std::make_shared<menu>(m_gameEngine);
		m_gameEngine->changeScene("menu" , sceneMenu); 
	}
	m_entities.update();
	spawnPlayer();
	if(!paused){
		sBackground();
		sGravity();
		sEnemySpawner();
		sMovement();
		sAi();
		sCollision();
		sAnimation();
	}
	sCamera();
	sCleanup();
	sRender();
};

void play::sDoAction(const action& actionName) {
	if (actionName.type == "Start") {
		if (actionName.name == "Jump") { m_player->getComponent<cInput>().up = 1; }
		if (actionName.name == "Duck") { m_player->getComponent<cInput>().down = 1; }
		if (actionName.name == "MoveRight") { m_player->getComponent<cInput>().right = 1; }
		if (actionName.name == "MoveLeft") { m_player->getComponent<cInput>().left = 1; }
		if (actionName.name == "Debug") { debugMode = !debugMode; }
		if (actionName.name == "Pause") { paused = !paused; }
		if (actionName.name == "Quit") { hasEnded = 1; }
	}
	else {
		if (actionName.name == "Jump") { m_player->getComponent<cInput>().up = 0; }
		if (actionName.name == "Duck") { m_player->getComponent<cInput>().down = 0; }
		if (actionName.name == "MoveRight") { m_player->getComponent<cInput>().right = 0; }
		if (actionName.name == "MoveLeft") { m_player->getComponent<cInput>().left = 0; }
		if (actionName.name == "Shoot") { spawnBullet(m_player); }
	}
};

void play::sBackground() {
	sf::Vector2f viewcenter = m_gameEngine->getWindow().getView().getCenter();
	if (viewcenter.x >= m_background.back()->getComponent<cTransform>().pos.x) {
		m_background.front()->getComponent<cTransform>().pos.x += m_background.back()->getComponent<cAnimation>().anim.getSize().x;
		m_background.push_back(m_background.front());
		m_background.pop_front();
		m_background.back()->getComponent<cAnimation>().anim.getSprite().scale(-1.0, 1);
	}
	if (viewcenter.x <= m_background.front()->getComponent<cTransform>().pos.x) {
		m_background.back()->getComponent<cTransform>().pos.x -= m_background.front()->getComponent<cAnimation>().anim.getSize().x;
		m_background.push_front(m_background.back());
		m_background.pop_back();
		m_background.front()->getComponent<cAnimation>().anim.getSprite().scale(-1.0, 1);
	}
}

void play::sRender() {
	m_gameEngine->getWindow().clear(sf::Color(100, 149, 237));
	sHud();

	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cAnimation>()) {
			if (e->hasComponent<cTransform>()) {
				Vec2& pos = e->getComponent<cTransform>().pos;
				e->getComponent<cAnimation>().anim.getSprite().setPosition(pos.x, pos.y);
			}
			m_gameEngine->getWindow().draw(e->getComponent<cAnimation>().anim.getSprite());
		}

		if (debugMode) {
			if (e->hasComponent<cBoundingBox>()) {
				sf::RectangleShape boundingBox;
				boundingBox.setSize(sf::Vector2f(e->getComponent<cBoundingBox>().dimensions.x, e->getComponent<cBoundingBox>().dimensions.y));
				boundingBox.setOrigin(boundingBox.getSize().x / 2, boundingBox.getSize().y / 2);
				boundingBox.setOutlineColor(sf::Color(255, 255, 255));
				boundingBox.setFillColor(sf::Color(255,255,255,0));
				boundingBox.setOutlineThickness(.5);
				boundingBox.setPosition(e->getComponent<cTransform>().pos.x, e->getComponent<cTransform>().pos.y);
				m_gameEngine->getWindow().draw(boundingBox);
			}
		}
	}


	m_gameEngine->getWindow().display();
};

void play::sAnimation() {

	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cAnimation>()) {
			int prevDir = e->getComponent<cAnimation>().anim.m_dir;

			if (e->hasComponent<cState>() ) {
				if (e->getComponent<cState>().state != e->getComponent<cAnimation>().anim.getName()) {
					e->addComponent<cAnimation>(m_gameEngine->getAssets().getAnimation(e->getComponent<cState>().state), 1);
				}
			}

			if (e->hasComponent<cTransform>()) {
				if (e->getComponent<cAnimation>().anim.m_dir * e->getComponent<cTransform>().vel.x < 0) {
					e->getComponent<cAnimation>().anim.getSprite().scale(-1, 1);
					e->getComponent<cAnimation>().anim.m_dir = -1 * e->getComponent<cAnimation>().anim.m_dir;
				}

				if (e->getComponent<cTransform>().vel.x == 0) {
					animation& animRef = e->getComponent<cAnimation>().anim;
					if (prevDir != animRef.m_dir) {
						animRef.getSprite().scale(-1, 1);
						animRef.m_dir = prevDir;
					}
				}
			}
		}
	}

	if (m_player->getComponent<cState>().state == "death") {
		m_player->getComponent<cAnimation>().looping = 0;
	}



	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cAnimation>()) {
			e->getComponent<cAnimation>().anim.update();
			if (!e->getComponent<cAnimation>().looping) {
				if (e->getComponent<cAnimation>().anim.hasEnded()) {
					std::cout << e->getTag() << "\n";
					e->setActive(0);
				}
			}
		}
	}
};

void play::sMovement() {

	if (m_player->getComponent<cState>().state != "death") {

		cInput& inputComp = m_player->getComponent<cInput>();
		cTransform& transformComp = m_player->getComponent<cTransform>();

		float yspeed = std::min(m_playerConfig.maxSpeed, transformComp.vel.y);
		yspeed = std::max(-m_playerConfig.maxSpeed , yspeed);
		transformComp.vel = { 0,yspeed };

		if (!inputComp.up && m_player->getComponent<cState>().state == "jumping"){
			transformComp.vel.y = std::max((float)0 , transformComp.vel.y);
		}


		if (inputComp.up ) {
			if (m_player->getComponent<cState>().state != "jumping") {
				transformComp.vel.y = -m_playerConfig.jump;
			}
		}
		if (inputComp.left) {
			transformComp.vel.x = -m_playerConfig.speed;
		}
		if (inputComp.right) {
			transformComp.vel.x = m_playerConfig.speed;
		}
		cState& stateComp = m_player->getComponent<cState>();
		stateComp.state = "standing";
		if (transformComp.vel.x != 0) {
			stateComp.state = "running";
		}
		if (inputComp.down) {
			m_player->getComponent<cState>().state = "ducking";
			transformComp.vel.x = 0;
		}
	}
	if (m_player->getComponent<cTransform>().pos == m_finalPoint) {
		hasEnded = 1;
	}
	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cTransform>()) {
			e->getComponent<cTransform>().prevPos = e->getComponent<cTransform>().pos;
			e->getComponent<cTransform>().pos += e->getComponent<cTransform>().vel;
		}
	}

};

void play::sGravity() {
	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cGravity>()) {
			e->getComponent<cTransform>().vel.y += m_playerConfig.gravity;
		}
	}
}

void play::sEnemySpawner() {
	//for (auto& enemy : m_entities.getEntities("enemy")) {
	//	enemy->getComponent<cTransform>().vel.x = m_playerConfig.speed * 0.4;
	//}
};



void play::sCollision() {
	entityVec movables = m_entities.getEntities("enemy");
	movables.push_back(m_player);
	if (m_player->getComponent<cInput>().down) {
		m_player->getComponent<cBoundingBox>().dimensions.y = m_playerConfig.cy/2.0;
	}
	else {
		m_player->getComponent<cBoundingBox>().dimensions.y = m_playerConfig.cy;
	}


	for (int i = 0; i < 3;i++) {
		for (auto& mover : movables) {
			if (mover->hasComponent<cState>() && mover->getComponent<cState>().state == "death") continue;
			if (!mover->hasComponent<cBoundingBox>())continue;
			int jump = 1;
			std::shared_ptr<entity> mxtile = nullptr;
			Vec2 mxoverlap = { 0,0 };


			for (auto& tile : m_entities.getEntities("tile")) {
				if (!tile->hasComponent<cBoundingBox>()) continue;
				cTransform& moverTrans = mover->getComponent<cTransform>();
				cTransform& tileTrans = tile->getComponent<cTransform>();
				Vec2 prevOverlap = getPrevOverlap(mover, tile);
				Vec2 overlap = getOverlap(tile, mover);
				if (overlap.x >= 0 && overlap.y >= 0) {
					if (prevOverlap.y <= 0) {
						if (mover->getComponent<cTransform>().prevPos.y < tile->getComponent<cTransform>().pos.y) {
							jump = 0;
						}
					}
				}

				if (overlap.x > 0 && overlap.y > 0) {
					if (mxoverlap.x * mxoverlap.y < overlap.x * overlap.y) {
						mxtile = tile;
						mxoverlap = overlap;
					}
				}
			}

			for (auto& tile : m_entities.getEntities("?block")) {
				if (!tile->hasComponent<cBoundingBox>()) continue;
				cTransform& moverTrans = mover->getComponent<cTransform>();
				cTransform& tileTrans = tile->getComponent<cTransform>();
				Vec2 prevOverlap = getPrevOverlap(mover, tile);
				Vec2 overlap = getOverlap(tile, mover);

				if (overlap.x >= 0 && overlap.y >= 0) {
					if (prevOverlap.y <= 0) {
						if (mover->getComponent<cTransform>().prevPos.y < tile->getComponent<cTransform>().pos.y) {
							jump = 0;
						}

					}
				}
				if (overlap.x > 0 && overlap.y > 0) {
					if (prevOverlap.y <= 0) {
						if (mover->getComponent<cTransform>().prevPos.y > tile->getComponent<cTransform>().pos.y) {
							auto t = m_entities.addentity("tile");
							spawnCoin(tile);
							replace(tile, t);
							t->getComponent<cAnimation>().anim.getSprite().setColor(sf::Color(200, 200, 200));
							mover->getComponent<cTransform>().vel.y = 0;
						}
					}
					if (mxoverlap.x * mxoverlap.y < overlap.x * overlap.y) {
						mxtile = tile;
						mxoverlap = overlap;
					}					
				}
			}

			for (auto& tile : m_entities.getEntities("destructible")) {
				if (!tile->hasComponent<cBoundingBox>())continue;
				cTransform& moverTrans = mover->getComponent<cTransform>();
				cTransform& tileTrans = tile->getComponent<cTransform>();
				Vec2 prevOverlap = getPrevOverlap(mover, tile);
				Vec2 overlap = getOverlap(tile, mover);
				if (overlap.x >= 0 && overlap.y >= 0) {
					if (prevOverlap.y <= 0) {
						if (mover->getComponent<cTransform>().prevPos.y < tile->getComponent<cTransform>().pos.y) {
							jump = 0;
						}
					}
				}

				if (overlap.x > 0 && overlap.y > 0) {
					if (mxoverlap.x * mxoverlap.y < overlap.x * overlap.y) {
						mxtile = tile;
						mxoverlap = overlap;
					}
				}
			}

			if(jump) if (mover->hasComponent<cState>()) mover->getComponent<cState>().state = "jumping";
			if (mxtile == nullptr) {
				if(i==0) if (mover->hasComponent<cState>()) mover->getComponent<cState>().state = "jumping";
				continue;
			}

			cTransform& moverTrans = mover->getComponent<cTransform>();
			cTransform& tileTrans = mxtile->getComponent<cTransform>();
			Vec2 prevOverlap = getPrevOverlap(mover, mxtile);

			if (prevOverlap.x <= 0) {
				int dir = moverTrans.prevPos.x > tileTrans.pos.x ? 1 : -1;
				moverTrans.pos.x += dir * mxoverlap.x;
			}
			else {
				int dir = moverTrans.prevPos.y > tileTrans.pos.y ? 1 : -1;
				moverTrans.vel.y = 0;
				moverTrans.pos.y += dir * mxoverlap.y;
			}
		}
	}




	for (auto &bullet : m_entities.getEntities("playerBullet")) {
		for (auto& enemy : m_entities.getEntities("enemy")) {
			if (!enemy->hasComponent<cBoundingBox>())continue;
			Vec2 overlap = getOverlap(bullet , enemy);
			if (overlap.x > 0 && overlap.y > 0) {
				bullet->setActive(0);
				destroy(enemy , m_gameEngine->getAssets().getAnimation("explosion"));
			}
		}
		for (auto& destTile : m_entities.getEntities("destructible")) {
			if (!destTile->hasComponent<cBoundingBox>())continue;
			Vec2 overlap = getOverlap(bullet, destTile);
			if (overlap.x > 0 && overlap.y > 0) {
				bullet->setActive(0);
				destroy(destTile , m_gameEngine->getAssets().getAnimation("explosion"));
			}
		}
		for (auto& tile : m_entities.getEntities("tile")) {
			Vec2 overlap = getOverlap(bullet, tile);
			if (overlap.x > 0 && overlap.y > 0) {
				bullet->setActive(0);
			}
		}
	}

	for (auto& enemy : m_entities.getEntities("enemy")) {
		if (enemy->getId() == m_player->getId()) continue;
		if (!enemy->hasComponent<cBoundingBox>())continue;
		if (!m_player->hasComponent<cBoundingBox>()) continue;
		Vec2 overlap = getOverlap(enemy , m_player);
		Vec2 prevOverlap = getPrevOverlap(enemy, m_player);
		if (overlap.x > 0 && overlap.y > 0) {
			if (prevOverlap.y <= 0) {
				if (m_player->getComponent<cTransform>().prevPos.y < enemy->getComponent<cTransform>().pos.y) {
					destroy(enemy , m_gameEngine->getAssets().getAnimation("enemy1Destroy"));
				}
				else {
					m_player->getComponent<cState>().state = "death";
				}
			}
			else {
				m_player->getComponent<cState>().state = "death";
			}
		}
	}
	

	for (auto& coin: m_entities.getEntities("coin")) {
		if (!coin->hasComponent<cBoundingBox>()) continue;
		Vec2 overlap = getOverlap(coin, m_player);
		if (overlap.x > 0 && overlap.y > 0) {
			coins++;
			coin->setActive(0);
		}
	}


};

void play::sCleanup() {
	Vec2 centre(m_gameEngine->getWindow().getView().getCenter().x , m_gameEngine->getWindow().getView().getCenter().y);
	Vec2 size(m_gameEngine->getWindow().getView().getSize().x , m_gameEngine->getWindow().getView().getSize().y);
	for (auto& bullet:m_entities.getEntities("playerBullets")) {
		if (!bullet->hasComponent<cBoundingBox>()) continue;
		Vec2 bulletPos = bullet->getComponent<cTransform>().pos;
		if (centre.x + size.x / 2 < bulletPos.x || centre.x - size.x / 2 > bulletPos.x || centre.y + size.y/2 < bulletPos.y || centre.y - size.y/2 > bulletPos.y) {
			bullet->setActive(0);
		}
	}

	for (auto& bullet : m_entities.getEntities("enemyBullets")) {
		if (!bullet->hasComponent<cBoundingBox>()) continue;
		Vec2 bulletPos = bullet->getComponent<cTransform>().pos;
		if (centre.x + size.x / 2 < bulletPos.x || centre.x - size.x / 2 > bulletPos.x || centre.y + size.y / 2 < bulletPos.y || centre.y - size.y / 2 > bulletPos.y) {
			bullet->setActive(0);
		}
	}



	for (auto &e : m_entities.getEntities()) {
		if (e->hasComponent<cLifeSpan>()) {
			e->getComponent<cLifeSpan>().rem--;
			if (e->getComponent<cLifeSpan>().rem <= 0) {
				e->setActive(0);
			}
		}
	}

	Vec2 playerPos = m_player->getComponent<cTransform>().pos;
	if ( centre.y + size.y / 2.0 < playerPos.y ) {
		m_player->getComponent<cState>().state = "death";
	}
}



void play::sCamera() {
	sf::RenderWindow& win = m_gameEngine->getWindow();
	sf::View view;
	view.setSize(win.getSize().x, win.getSize().y);
	view.setCenter(m_player->getComponent<cTransform>().pos.x , win.getSize().y/2.0);
	win.setView(view);
}


void play::sAi() {


	for (auto& mover : m_entities.getEntities("enemy")) {
		if (mover->hasComponent<cState>() && mover->getComponent<cState>().state == "death") continue;
		if (!mover->hasComponent<cBoundingBox>())continue;
		std::shared_ptr<entity> mxtile = nullptr;
		Vec2 mxoverlap = { 0,0 };


		for (auto& tile : m_entities.getEntities("tile")) {
			if (!tile->hasComponent<cBoundingBox>()) continue;
			cTransform& moverTrans = mover->getComponent<cTransform>();
			cTransform& tileTrans = tile->getComponent<cTransform>();
			Vec2 prevOverlap = getPrevOverlap(mover, tile);
			Vec2 overlap = getOverlap(tile, mover);
			if (overlap.x > 0 && overlap.y > 0) {
				if (mxoverlap.x * mxoverlap.y < overlap.x * overlap.y) {
					mxtile = tile;
					mxoverlap = overlap;
				}
			}
		}

		for (auto& tile : m_entities.getEntities("?block")) {
			if (!tile->hasComponent<cBoundingBox>()) continue;
			cTransform& moverTrans = mover->getComponent<cTransform>();
			cTransform& tileTrans = tile->getComponent<cTransform>();
			Vec2 prevOverlap = getPrevOverlap(mover, tile);
			Vec2 overlap = getOverlap(tile, mover);
			if (overlap.x > 0 && overlap.y > 0) {
				if (mxoverlap.x * mxoverlap.y < overlap.x * overlap.y) {
					mxtile = tile;
					mxoverlap = overlap;
				}
			}
		}

		for (auto& tile : m_entities.getEntities("destructible")) {
			if (!tile->hasComponent<cBoundingBox>())continue;
			cTransform& moverTrans = mover->getComponent<cTransform>();
			cTransform& tileTrans = tile->getComponent<cTransform>();
			Vec2 prevOverlap = getPrevOverlap(mover, tile);
			Vec2 overlap = getOverlap(tile, mover);
			if (overlap.x > 0 && overlap.y > 0) {
				if (mxoverlap.x* mxoverlap.y < overlap.x* overlap.y) {
					mxtile = tile;
					mxoverlap = overlap;
				}
			}
		}

		if (mxtile == nullptr) {
			continue;
		}

		cTransform& moverTrans = mover->getComponent<cTransform>();
		cTransform& tileTrans = mxtile->getComponent<cTransform>();
		Vec2 prevOverlap = getPrevOverlap(mover, mxtile);

		if (prevOverlap.x <= 0) {
			mover->getComponent<cTransform>().vel.x *= -1;
		}
	}
}


void play::sHud() {
	std::shared_ptr<entity> coin = m_entities.getEntities("hudCoin")[0];

	Vec2 centre(m_gameEngine->getWindow().getView().getCenter().x, m_gameEngine->getWindow().getView().getCenter().y);
	Vec2 size(m_gameEngine->getWindow().getView().getSize().x, m_gameEngine->getWindow().getView().getSize().y);
	float length = centre.x - size.x / 2;
	float width = 0;
	for (auto& heart : m_entities.getEntities("hudHeart")) {
		length = length + heart->getComponent<cAnimation>().anim.getSize().x;
		heart->getComponent<cTransform>().pos.x = length;
		width = centre.y - size.y / 2 + heart->getComponent<cAnimation>().anim.getSize().y / 2 * 2;
		heart->getComponent<cTransform>().pos.y = centre.y - size.y / 2 + coin->getComponent<cAnimation>().anim.getSize().y / 2 * 2;
	}
	length = length + coin->getComponent<cAnimation>().anim.getSize().x;
	coin->getComponent<cTransform>().pos.x = length;
	coin->getComponent<cTransform>().pos.y = width;

	sf::Text cointCount;
	cointCount.setFont(m_gameEngine->getAssets().getFont("menuFont"));

	cointCount.setString(std::to_string(coins));
	cointCount.setFillColor(sf::Color::White);
	cointCount.setPosition((int)length + (int)coin->getComponent<cAnimation>().anim.getSize().x/2 , (int)0);
	cointCount.setCharacterSize(coin->getComponent<cAnimation>().anim.getSize().y * 1.5);
	m_gameEngine->getWindow().draw(cointCount);
}


void play::sDebug() {
	cInput& inputComp = m_player->getComponent<cInput>();
	cTransform& transformComp = m_player->getComponent<cTransform>();
	transformComp.vel = { 0,0 };

	if (inputComp.up) {
		transformComp.vel.y = -m_playerConfig.speed;
	}
	if (inputComp.left) {
		transformComp.vel.x = -m_playerConfig.speed;
	}
	if (inputComp.right) {
		transformComp.vel.x = m_playerConfig.speed;
	}
	if (inputComp.down) {
		transformComp.vel.y = m_playerConfig.speed;
	}

	for (auto& e : m_entities.getEntities()) {
		if (e->hasComponent<cTransform>()) {
			e->getComponent<cTransform>().prevPos = e->getComponent<cTransform>().pos;
			e->getComponent<cTransform>().pos += e->getComponent<cTransform>().vel;
		}
	}
};