#include "components.h"

cGravity::cGravity(float x):
	acceleration(x)
	{}

cTransform::cTransform(const Vec2& pos,const Vec2& vel) :
	vel(vel),
	pos(pos),
	angle(0),
	prevPos(pos)
{
}

cBoundingBox::cBoundingBox(const Vec2& dim):
	dimensions(dim)
{
}

cLifeSpan::cLifeSpan(float time):
	total(time),
	rem(time)
{
}

cInput::cInput():
	up(0), down(0), left(0), right(0), mouseLeft(0), mouseRight(0)
{
}

cGravity::cGravity()
{
}

cTransform::cTransform():
	vel(0,0),
	pos(0,0),
	angle(0),
	prevPos(0,0) {
}

cBoundingBox::cBoundingBox()
{
}

cLifeSpan::cLifeSpan()
{
}

cAnimation::cAnimation():
	anim()
{
};

cAnimation::cAnimation(const animation& anim, bool loop) :
	anim(anim),
	looping(loop)
{
}

cState::cState() {
};
