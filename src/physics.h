#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "util.h"
#include "clock.h"

class Physics {
public:
	Physics();
	~Physics();

	void init(Clock& clock);

	glm::vec3 getCameraPosition(glm::vec3 lastPos, glm::mat4 yawRotM, glm::vec3 velocity, float scale);

private:
	Clock* clockPtr_ = nullptr;


};