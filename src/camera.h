#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

#include "types.h"
#include "util.h"

class Camera {

public:
	Camera();
	~Camera();

	void init(float aspect);

	void processEvent(const SDL_Event& event, const KeyStates& keys);

	void update(float aspect);
	
	void cleanup();

	const glm::mat4& getViewProj() const;
	const glm::mat4& getPerspectiveProj() const;

private:
	float fovy_ = 0;
	float near_ = 0;
	float far_ = 0;

	glm::vec3 velocity_ = { 0,0,0 };
	glm::vec3 position_ = {0,0,0};
	
	// verticle rotation
	float pitch_ = 0;
	// horizontal rotation
	float yaw_ = 0;

	glm::mat4 view_ = glm::mat4(1);
	glm::mat4 persp_ = glm::mat4(1);

	glm::mat4 getRotM();


};