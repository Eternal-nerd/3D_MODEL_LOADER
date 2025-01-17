#include "camera.h"

Camera::Camera() {}
Camera::~Camera() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Camera::init(float aspect) {
	// Set initial position of the camera
	position_ = { 0, 1, 1 };

	// Set position to start looking at
	lookAt_ = { 0, 0, 0 };

	// set rotation
	rotation_ = { 0,0,0 };

	// last argument is "up" or direction up vector
	view_ = glm::lookAt(position_, lookAt_, glm::vec3(0.0f, 1.0f, 0.0f));

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
	if (keys.w) {
		position_[2] -= 0.1;
	}
	if (keys.a) {
		position_[0] -= 0.1;
	}
	if (keys.s) {
		position_[2] += 0.1;
	}
	if (keys.d) {
		position_[0] += 0.1;
	}

	std::cout << "position_: " << glm::to_string(position_) << "\n";

	// Translation matrix
	//glm::mat4 transM(1);
	//std::cout << "transM before: " << glm::to_string(transM) << "\n";
	//transM = glm::translate(glm::mat4(1.0f), position_);
	//std::cout << "transM  after: " << glm::to_string(transM) << "\n";

	// Rotation matrix
	//glm::mat4 rotM = glm::mat4(1);

	// DO ROTATION FROM MOUSE INPUT HERE:


	//std::cout << "view_        : " << glm::to_string(view_) << "\n";
	//std::cout << "view_ * transM: " << glm::to_string(view_ * transM) << "\n";
	// FIXMEEEE
	view_ = glm::lookAt(position_, lookAt_, glm::vec3(0.0f, 1.0f, 0.0f));

	//throw std::runtime_error("end");

	persp_ = glm::perspective(glm::radians(fovy_), aspect, near_, far_);
	persp_[1][1] *= -1;
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

