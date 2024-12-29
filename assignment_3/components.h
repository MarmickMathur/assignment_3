#pragma once
#include "Vec2.h"
#include "animation.h"
class component {
public:
	bool has = false;
};


class cTransform:public component {
public:
	Vec2 vel = {0 , 0};
	Vec2 pos = {0 , 0};
	Vec2 prevPos = { 0 , 0};
	float angle = 0;
	cTransform();
	cTransform(const Vec2& pos ,const Vec2& vel);
};

class cBoundingBox:public component {
public:
	Vec2 dimensions = {0 , 0};
	cBoundingBox();
	cBoundingBox(const Vec2& dim);
};

class cLifeSpan :public component {
public:
	float total = 0;
	float rem = 0;
	cLifeSpan();
	cLifeSpan(float t);
};

class cInput :public component {
public:
	bool up = 0;
	bool down = 0;
	bool left = 0;
	bool right = 0;
	bool mouseLeft = 0;
	bool mouseRight = 0;
	cInput();
};

class cAnimation :public component {
public:
	bool looping = 0;
	animation anim;
	cAnimation();
	cAnimation(const animation& anim, bool loop);
};

class cState :public component {
public:
	std::string state;
	cState();
};

class cGravity :public component {
public:
	float acceleration = 0.0;
	cGravity();
	cGravity(float x);
};
