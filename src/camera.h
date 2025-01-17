#pragma once

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#include "types.h"

class Camera {

public:
	Camera();
	~Camera();

	void init(float aspect);

	void update(const KeyStates& keys, float aspect);
	
	void cleanup();

	const glm::mat4& getViewProj() const;
	const glm::mat4& getPerspectiveProj() const;

private:
	float fovy_ = 0;
	float near_ = 0;
	float far_ = 0;

	glm::vec3 position_ = {0,0,0};
	glm::vec3 lookAt_ = { 0,0,0 };
	glm::vec3 rotation_ = { 0,0,0 };

	glm::mat4 view_ = glm::mat4(1);
	glm::mat4 persp_ = glm::mat4(1);


};