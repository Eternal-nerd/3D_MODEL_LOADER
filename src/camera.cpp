#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init() {}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Camera::update() {}


/*-----------------------------------------------------------------------------
-----------------------------GET-MATRICES--------------------------------------
-----------------------------------------------------------------------------*/
const glm::mat4& Camera::getViewProj() const {
	// FIXME
	return glm::mat4(1);
}

const glm::mat4& Camera::getPerspectiveProj() const {
	// FIXME
	return glm::mat4(1);
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Camera::cleanup() {}

