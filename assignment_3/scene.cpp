#include "scene.h"

scene::scene(const std::shared_ptr<gameEngine> ge):
	paused(0),
	hasEnded(0),
	currentFrame(0),
	m_entities(),
	m_gameEngine(ge)
{
}

void scene::simulate() {
}
void scene::doAction(const action& action) {
	sDoAction(action);
};

void scene::registerAction(int inputCode , const std::string actionName) {
	m_actionMap[inputCode] = actionName;
};

ActionMap scene::getActionMap() {
	return m_actionMap;
}

