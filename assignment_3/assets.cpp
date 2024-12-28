#include "assets.h"


void assets::addfont(const std::string& name,const std::string& path) {
	std::shared_ptr<sf::Font> font= std::make_shared<sf::Font>();
	font->loadFromFile(path);
	m_fontMap[name] = font;
};

std::shared_ptr<sf::Font> assets::getFont(const std::string& name) {
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