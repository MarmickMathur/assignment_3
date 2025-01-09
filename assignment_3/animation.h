#pragma once
#include <SFML/Graphics.hpp>
#include "Vec2.h"

class animation {
	sf::Sprite m_sprite;
	int m_frameCountx;
	int m_frameCounty;
	int m_holdFrameFor;
	int m_framePos;
	int m_frameEnd;
	int m_frameStart;
	Vec2 m_size;
	int m_currentFrame;
	std::string m_name;
public:
	int m_dir;
	animation();
	animation(const std::string& name, sf::Texture& tex, int holdFrameFor = 1e8, int frameCountx = 1, int frameCountY = 1, int framePos = 0, int frameEnd = 0 , int frameStart = 0);
	void update();
	bool hasEnded() const;
	std::string& getName();
	Vec2& getSize();
	sf::Sprite& getSprite();
};