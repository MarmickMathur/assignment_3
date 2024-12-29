#include "assets.h"


void assets::addfont(const std::string& name,const std::string& path) {
	sf::Font font;
	font.loadFromFile(path);
	m_fontMap[name] = font;
};

sf::Font& assets::getFont(const std::string& name) {
	return m_fontMap[name];
};

std::string& assets::getLevelPath(const std::string& name) {
	return m_LevelPathMap[name];
}

LevelMap& assets::getLevelPathMap() {
	return m_LevelPathMap;
}

void assets::addLevel(const std::string& name, const std::string& path) {
	m_LevelPathMap[name] = path;
}

void assets::addTexture(const std::string& name, const std::string& path) {
	sf::Texture tex;
	tex.loadFromFile(path);
	m_textures[name] = tex;
}

sf::Texture& assets::getTexture(const std::string& name) {
	return m_textures[name];
}

animation& assets::getAnimation(const std::string& name) {
	return m_animationMap[name];
}

void assets::addAnimation(const std::string& name, const animation& anim) {
	m_animationMap[name] = anim;
};

