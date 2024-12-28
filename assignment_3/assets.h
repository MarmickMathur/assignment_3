#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
typedef std::map<std::string, std::shared_ptr<sf::Font>> FontMap;
typedef std::map<std::string, std::string> LevelMap;


class assets {
	FontMap m_fontMap;
	LevelMap m_LevelPathMap;
public:
	void addfont(const std::string& name,const std::string& path);
	std::shared_ptr<sf::Font> getFont(const std::string& name);
	void addLevel(const std::string& name, const std::string& path);
	std::string& getLevelPath(const std::string& name);
	LevelMap& getLevelPathMap();
};