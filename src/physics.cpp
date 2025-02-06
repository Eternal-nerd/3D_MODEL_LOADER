#include "physics.h"

Physics::Physics() {}
Physics::~Physics() {}

void Physics::init(Clock& clock) {
	clockPtr_ = &clock;
}

glm::vec3 Physics::getCameraPosition(glm::vec3 lastPos, glm::mat4 yawRotM, glm::vec3 velocity, float scale) {


	scale *= 1.5f;
	return lastPos + glm::vec3(yawRotM * glm::vec4(velocity * scale, 0));
}
