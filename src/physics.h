#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "util.h"
#include "clock.h"
#include "types.h"

// m/s^2
const float GRAVITY_ACC = -9.80665;
const float JUMP_ACC = 99.f;

// Meters/second
const float MAX_CAMERA_VELOCITY = 15.f;
const float NOCLIP_SPEED = 30.f;
// meters



class Physics {
public:
	Physics();
	~Physics();

	void init(Clock& clock, KeyStates& keys);

	void updateCameraPos(CamPhysicsAttributes& camPhys, glm::mat4 yawRotM, bool noclip);

private:
	Clock* clockPtr_ = nullptr;
	float lastCamUpdate_ = 0.f;

	KeyStates* keysPtr_ = nullptr;

	void applyCamVelocity(CamPhysicsAttributes& camPhys, float deltaT, bool noclip);

};