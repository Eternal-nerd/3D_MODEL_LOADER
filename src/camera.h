#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <cmath>

#include <iostream>

#include "types.h"
#include "util.h"
#include "clock.h"
#include "physics.h"

class Camera {

public:
	Camera();
	~Camera();

	void init(float aspect, CamPhysics& physics);

	void processEvent(const SDL_Event& event, const KeyStates& keys);

	void update(float aspect);
	
	void cleanup();

	glm::vec3 getPosition();
	glm::vec3 getVelocity();

	const glm::mat4& getViewProj() const;
	const glm::mat4& getPerspectiveProj() const;

private:
	// reference to physics
	CamPhysics* physicsPtr_ = nullptr;

	float fovy_ = 0;
	float near_ = 0;
	float far_ = 0;

	CamPhysicsAttributes phys_;
	bool noclip_ = false;
	
	// verticle rotation
	float pitch_ = 0;
	// horizontal rotation
	float yaw_ = 0;

	glm::mat4 view_ = glm::mat4(1);
	glm::mat4 persp_ = glm::mat4(1);

	glm::mat4 getRotM();

	glm::mat4 getYawRotM();

};