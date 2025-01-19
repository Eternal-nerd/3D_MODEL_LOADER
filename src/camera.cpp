#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init(float aspect) {
	// Set initial position of the camera
	position_ = { 0, 0, 2 };

	// set intial rotation
	pitch_ = 0;
	yaw_ = 0;

	// init view matrix
	view_ = glm::translate(glm::mat4(1), position_);

	// set camera FOV:
	fovy_ = 90;

	// set near plane:
	near_ = 0.1;

	// set far plane:
	far_ = 100;

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	// invert the Y direction on projection matrix ???
	persp_[1][1] *= -1;
}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Camera::update(float aspect) {
	// get rotation matrix from mouse input
	glm::mat4 rotM = getRotM();

	// update position
	position_ += glm::vec3(rotM * glm::vec4(velocity_ * 0.2f, 0));
	glm::mat4 transM = glm::translate(glm::mat4(1), position_);

	// apply updates to view matrix
	view_ = glm::inverse(transM * rotM);

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	persp_[1][1] *= -1;
}

void Camera::processEvent(const SDL_Event& event, const KeyStates& keys) {
	// TODO Used key States struct to improve movement

	switch (event.type) {
	// KEYBOARD EVENTS
	case SDL_EVENT_KEY_DOWN:
		if (event.key.scancode == SDL_SCANCODE_W) { velocity_.z = -1; }
		if (event.key.scancode == SDL_SCANCODE_A) { velocity_.x = -1; }
		if (event.key.scancode == SDL_SCANCODE_S) { velocity_.z = 1; }
		if (event.key.scancode == SDL_SCANCODE_D) { velocity_.x = 1; }
		if (event.key.scancode == SDL_SCANCODE_SPACE) { velocity_.y = 1; }
		if (event.key.scancode == SDL_SCANCODE_LCTRL) { velocity_.y = -1; }
		break;
	case SDL_EVENT_KEY_UP:
		if (event.key.scancode == SDL_SCANCODE_W) { velocity_.z = 0; }
		if (event.key.scancode == SDL_SCANCODE_A) { velocity_.x = 0; }
		if (event.key.scancode == SDL_SCANCODE_S) { velocity_.z = 0; }
		if (event.key.scancode == SDL_SCANCODE_D) { velocity_.x = 0; }
		if (event.key.scancode == SDL_SCANCODE_SPACE) { velocity_.y = 0; }
		if (event.key.scancode == SDL_SCANCODE_LCTRL) { velocity_.y = 0; }
		break;
	// MOUSE EVENTS
	case SDL_EVENT_MOUSE_MOTION:
		yaw_ += (float)event.motion.xrel / 200;
		pitch_ -= (float)event.motion.yrel / 200;
		break;
	}
}

glm::mat4 Camera::getRotM() {
	glm::quat pitchRotation = glm::angleAxis(pitch_, glm::vec3{ 1, 0, 0 });
	glm::quat yawRotation = glm::angleAxis(yaw_, glm::vec3{ 0, -1, 0 });

	return glm::toMat4(yawRotation) * glm::toMat4(pitchRotation);
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

