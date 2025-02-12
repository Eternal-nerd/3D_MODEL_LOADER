#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init(float aspect, Physics& physics) {
	// set access ptr to clock
	physicsPtr_ = &physics;

	// Set initial position of the camera
	phys_.position = { 0, 2, 2 };

	// set intial rotation
	pitch_ = 0;
	yaw_ = 0;

	// init view matrix
	view_ = glm::translate(glm::mat4(1), phys_.position);

	// set camera FOV:
	fovy_ = 90;

	// set near plane:
	near_ = 0.1f;

	// set far plane:
	far_ = 2000; // FIXME

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	// invert the Y direction on projection matrix ???
	persp_[1][1] *= -1;
}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Camera::update(float aspect) {
	physicsPtr_->updateCameraPos(phys_, getYawRotM(), noclip_);

	glm::mat4 transM = glm::translate(glm::mat4(1), phys_.position);

	// get rotation matrix from mouse input
	glm::mat4 rotM = getRotM();

	// apply updates to view matrix
	view_ = glm::inverse(transM * rotM);

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	persp_[1][1] *= -1;
}

void Camera::processEvent(const SDL_Event& event, const KeyStates& keys) {
	switch (event.type) {
	// KEYBOARD EVENTS
	case SDL_EVENT_KEY_DOWN:
			if (event.key.scancode == SDL_SCANCODE_N) { noclip_ = !noclip_; }
			break;
	case SDL_EVENT_KEY_UP:
			break;

	// MOUSE EVENTS
	case SDL_EVENT_MOUSE_MOTION:
		yaw_ += (float)event.motion.xrel / 250;

		// limit pitch to +-90 degrees
		if (abs(pitch_ - (float)event.motion.yrel / 200) < glm::radians(90.0f)) {
			pitch_ -= (float)event.motion.yrel / 250;
		}
		
		break;
	}
}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/
glm::vec3 Camera::getPosition() {
	return phys_.position;
}

glm::vec3 Camera::getVelocity() {
	return phys_.velocity;
}

glm::mat4 Camera::getRotM() {
	glm::quat pitchRotation = glm::angleAxis(pitch_, glm::vec3{ 1, 0, 0 });
	glm::quat yawRotation = glm::angleAxis(yaw_, glm::vec3{ 0, -1, 0 });

	return glm::toMat4(yawRotation) * glm::toMat4(pitchRotation);
}

glm::mat4 Camera::getYawRotM() {
	glm::quat yawRotation = glm::angleAxis(yaw_, glm::vec3{ 0, -1, 0 });
	return glm::toMat4(yawRotation);
}

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

