#pragma once
#include "Vec2.h"

class cTransform {
public:
	Vec2 vel;
	Vec2 pos;
	Vec2 prevPos;
	float angle;
	cTransform(Vec2 pos , Vec2 vel);
};

class cBoundingBox {
public:
	Vec2 dimensions;
	cBoundingBox(Vec2 inp);
};

class cLifeSpan {
public:
	float total,rem;
	cLifeSpan(float t);
};

class cInput {
	bool up, down, left, right, mouseLeft, mouseRight;
	cInput();
};

class cAnimation {

};

class cState {

};

class cGravity {
public:
	float acceleration = { 0 };
	cGravity(float x);
};
