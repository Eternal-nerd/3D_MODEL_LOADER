#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init(float aspect, Clock& clock) {
	// set access ptr to clock
	clockPtr_ = &clock;

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
	near_ = 0.1f;

	// set far plane:
	far_ = 100;

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	// invert the Y direction on projection matrix ???
	persp_[1][1] *= -1;

	lastUpdate_ = clockPtr_->getProgramSeconds();
}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Camera::update(float aspect) {
	// get time difference from last update
	float delta = clockPtr_->getProgramSeconds() - lastUpdate_;

	// USE Time delta to scale position transformation
	float timeMult = delta * 10.f;

	float sprintMult = sprint_ ? 3.f : 1.f;

	float scale = timeMult * sprintMult;

	// update position
	position_ += glm::vec3(getYawRotM() * glm::vec4(velocity_ * scale, 0));
	glm::mat4 transM = glm::translate(glm::mat4(1), position_);

	// get rotation matrix from mouse input
	glm::mat4 rotM = getRotM();

	// apply updates to view matrix
	view_ = glm::inverse(transM * rotM);

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	persp_[1][1] *= -1;
	
	lastUpdate_ = clockPtr_->getProgramSeconds();
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
		if (event.key.scancode == SDL_SCANCODE_LSHIFT) { sprint_ = true; }
		break;
	case SDL_EVENT_KEY_UP:
		if (event.key.scancode == SDL_SCANCODE_W) { velocity_.z = 0; }
		if (event.key.scancode == SDL_SCANCODE_A) { velocity_.x = 0; }
		if (event.key.scancode == SDL_SCANCODE_S) { velocity_.z = 0; }
		if (event.key.scancode == SDL_SCANCODE_D) { velocity_.x = 0; }
		if (event.key.scancode == SDL_SCANCODE_SPACE) { velocity_.y = 0; }
		if (event.key.scancode == SDL_SCANCODE_LCTRL) { velocity_.y = 0; }
		if (event.key.scancode == SDL_SCANCODE_LSHIFT) { sprint_ = false; }
		break;
	// MOUSE EVENTS
	case SDL_EVENT_MOUSE_MOTION:
		yaw_ += (float)event.motion.xrel / 200;

		// limit pitch to +-90 degrees
		if (abs(pitch_ - (float)event.motion.yrel / 200) < glm::radians(90.0f)) {
			pitch_ -= (float)event.motion.yrel / 200;
		}
		
		break;
	}
}

/*-----------------------------------------------------------------------------
-----------------------------GET-MATRICES--------------------------------------
-----------------------------------------------------------------------------*/
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

