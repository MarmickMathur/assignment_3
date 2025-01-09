#include "animation.h"
#include <iostream>

animation::animation(const std::string& name, sf::Texture& tex, int holdFrameFor, int frameCountx, int frameCounty, int framePos, int frameEnd , int frameStart) :
	m_name(name),
	m_holdFrameFor(holdFrameFor),
	m_frameCountx(frameCountx),
	m_frameCounty(frameCounty),
	m_framePos(framePos),
	m_currentFrame(0),
	m_frameEnd(frameEnd),
	m_frameStart(frameStart),
	m_dir(1)
{

	//std::cout << m_name << "\n";
	m_sprite.setTexture(tex);
	m_size.x = tex.getSize().x / (float)frameCountx;
	m_size.y = tex.getSize().y / (float)frameCounty;
	m_sprite.setOrigin(m_size.x/2 , m_size.y/2);
}

animation::animation():
	m_name(),
	m_holdFrameFor(),
	m_frameCountx(),
	m_frameCounty(),
	m_framePos(),
	m_currentFrame(0)
{}

void animation::update() {
	int frameLen = m_frameCountx - m_frameEnd - m_frameStart;
	float left = (float)((int)(m_currentFrame / m_holdFrameFor) % (frameLen) + m_frameStart) * m_size.x;
	float top = m_size.y * ((float)m_framePos);
	m_sprite.setTextureRect(sf::IntRect(left, top, m_size.x,m_size.y ));
	m_currentFrame++;

	if (m_currentFrame > m_holdFrameFor * frameLen) {
		m_currentFrame = 0;
	}
}

bool animation::hasEnded() const {
	int frameLen = m_frameCountx - m_frameEnd - m_frameStart;
	if (m_currentFrame >= m_holdFrameFor * frameLen) {
		return 1;
	}
	return 0;
}

std::string& animation::getName() {
	return m_name;
}

Vec2& animation::getSize() {
	return m_size;
}

sf::Sprite& animation::getSprite() {
	return m_sprite;
}