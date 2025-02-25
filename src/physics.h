#pragma once

#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>

#include "util.h"
#include "clock.h"
#include "types.h"
#include "audio.h"

// m/s^2
const float GRAVITY_ACC = -9.80665;
const float JUMP_ACC = 99.f;

// Meters/second
const float MAX_CAMERA_VELOCITY = 15.f;
const float NOCLIP_SPEED = 30.f;
// meters

class ObjectPhysics {
public:



private:



};



class CamPhysics {
public:
	CamPhysics();
	~CamPhysics();

	void init(Clock& clock, KeyStates& keys, Audio& audio);

	void updateCameraPos(CamPhysicsAttributes& camPhys, glm::mat4 yawRotM, bool noclip);

private:
	Clock* clockPtr_ = nullptr;
	float lastCamUpdate_ = 0.f;

	KeyStates* keysPtr_ = nullptr;

	Audio* audio_ = nullptr;

	void applyCamVelocity(CamPhysicsAttributes& camPhys, float deltaT, bool noclip);

};