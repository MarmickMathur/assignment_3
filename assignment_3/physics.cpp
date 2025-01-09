#include "physics.h"
#include <iostream>

Vec2 getOverlap(std::shared_ptr<entity> e1, std::shared_ptr<entity> e2) {
	float diffx = abs(e1->getComponent<cTransform>().pos.x - e2->getComponent<cTransform>().pos.x);
	float overlapx = (e1->getComponent<cBoundingBox>().dimensions.x + e2->getComponent<cBoundingBox>().dimensions.x)/2 - diffx;

	float diffy = abs(e1->getComponent<cTransform>().pos.y - e2->getComponent<cTransform>().pos.y);
	float overlapy = (e1->getComponent<cBoundingBox>().dimensions.y + e2->getComponent<cBoundingBox>().dimensions.y)/2 - diffy;

	return { overlapx , overlapy };
}

Vec2 getPrevOverlap(std::shared_ptr<entity> e1, std::shared_ptr<entity> e2) {
	float diffx = abs(e1->getComponent<cTransform>().prevPos.x - e2->getComponent<cTransform>().prevPos.x);
	float overlapx = (e1->getComponent<cBoundingBox>().dimensions.x + e2->getComponent<cBoundingBox>().dimensions.x)/2 - diffx;

	float diffy = abs(e1->getComponent<cTransform>().prevPos.y - e2->getComponent<cTransform>().prevPos.y);
	float overlapy = (e1->getComponent<cBoundingBox>().dimensions.y + e2->getComponent<cBoundingBox>().dimensions.y)/2 - diffy;

	return { overlapx , overlapy };
}