#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "animation.h"
typedef std::map<std::string, sf::Font> FontMap;
typedef std::map<std::string, sf::Texture> TextureMap;
typedef std::map<std::string, std::string> LevelMap;
typedef std::map<std::string, animation> AnimationMap;


class assets {
	FontMap m_fontMap;
	LevelMap m_LevelPathMap;
	TextureMap m_textures;
	AnimationMap m_animationMap;

public:
	void addfont(const std::string& name,const std::string& path);
	sf::Font& getFont(const std::string& name);

	void addLevel(const std::string& name, const std::string& path);
	std::string& getLevelPath(const std::string& name);
	LevelMap& getLevelPathMap();

	sf::Texture& getTexture(const std::string& name);
	void addTexture(const std::string& name ,const std::string& path);

	animation& getAnimation(const std::string& name);
	void addAnimation(const std::string& name,const animation& anim);
};