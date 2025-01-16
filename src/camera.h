#pragma once

#include <glm/glm.hpp>


class Camera {

public:
	Camera();
	~Camera();

	void init();
	void update();
	void cleanup();

	const glm::mat4& getViewProj() const;
	const glm::mat4& getPerspectiveProj() const;

private:



};