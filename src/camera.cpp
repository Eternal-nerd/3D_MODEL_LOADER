#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init(float aspect) {
	// Set initial position of the camera
	position_ = { 0, 0, -2 };

	// set intial rotation
	rotation_ = { 0,0,0 };

	// init view matrix
	view_ = glm::translate(glm::mat4(1), position_);

	// set camera FOV:
	fovy_ = 90;

	// set near plane:
	near_ = 0.1;

	// set far plane:
	far_ = 100;

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	// TODO WHATS THIS LINE FOR?
	persp_[1][1] *= -1;
}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Camera::update(const KeyStates& keys, float aspect) {
	// apply camera position updates and update matrices
	updatePosition(keys);

	view_ = glm::translate(glm::mat4(1), position_);

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	persp_[1][1] *= -1;
}

void Camera::updatePosition(const KeyStates& keys) {
	float moveIncr = 0.2;
	float shiftMult = 1;
	if (keys.shift) {
		shiftMult = 4;
	}

	if (keys.w) {
		position_[2] += moveIncr * shiftMult;
	}
	if (keys.a) {
		position_[0] += moveIncr * shiftMult;
	}
	if (keys.s) {
		position_[2] -= moveIncr * shiftMult;
	}
	if (keys.d) {
		position_[0] -= moveIncr * shiftMult;
	}
	if (keys.space) {
		position_[1] -= moveIncr * shiftMult;
	}
	if (keys.ctrl) {
		position_[1] += moveIncr * shiftMult;
	}
}


/*-----------------------------------------------------------------------------
-----------------------------GET-MATRICES--------------------------------------
-----------------------------------------------------------------------------*/
const glm::mat4& Camera::getViewProj() const {
	return view_;
}

const glm::mat4& Camera::getPerspectiveProj() const {
	return persp_;
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Camera::cleanup() {}

