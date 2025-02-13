#include "physics.h"

Physics::Physics() {}
Physics::~Physics() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Physics::init(Clock& clock, KeyStates& keys, Audio& audio) {
	clockPtr_ = &clock;
	keysPtr_ = &keys;
	audio_ = &audio;
	lastCamUpdate_ = clockPtr_->getProgramSeconds();
}

/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Physics::updateCameraPos(CamPhysicsAttributes& camPhys, glm::mat4 yawRotM, bool noclip) {
	// get time difference from last update
	float delta = clockPtr_->getProgramSeconds() - lastCamUpdate_;

	// apply velocity updates
	applyCamVelocity(camPhys, delta, noclip);

	// update position
	glm::vec3 positionIncr = glm::vec3(yawRotM * glm::vec4(camPhys.velocity * delta, 0));
	camPhys.position += positionIncr;
	if (!noclip) {
		// movement
		camPhys.distanceSinceStep += std::max(std::abs(positionIncr.x), std::abs(positionIncr.z));
		if (camPhys.distanceSinceStep > 3.f) {
			// play step sound if grounded:
			if (camPhys.grounded) {
				// play random step sound, 5 to choose from
				int index = 2 + (rand() % 5);
				audio_->play(index);
			}
			// reset it
			camPhys.distanceSinceStep = 0.f;
		}


		if (camPhys.grounded && camPhys.position.y > 2.f) {
			audio_->play(0);
		}
		if (!camPhys.grounded && camPhys.position.y <= 2.f) {
			audio_->play(1);
		}
	}

	// FIXME this is poor man's collision detection
	camPhys.grounded = camPhys.position.y <= 2.f;

	lastCamUpdate_ = clockPtr_->getProgramSeconds();
}

void Physics::applyCamVelocity(CamPhysicsAttributes& camPhys, float deltaT, bool noclip) {
	float sprintDiv = keysPtr_->shift ? 1.f : 3.f;

	// TODO Noclip mode
	if (noclip) {
		// Z DIRECTION
		if (keysPtr_->w == keysPtr_->s) {
			camPhys.velocity.z = 0;
		}
		if (keysPtr_->w) {
			camPhys.velocity.z = -NOCLIP_SPEED / sprintDiv;
		}
		if (keysPtr_->s) {
			camPhys.velocity.z = NOCLIP_SPEED / sprintDiv;
		}
		// X DIRECTION
		if (keysPtr_->a == keysPtr_->d) {
			camPhys.velocity.x = 0;
		}
		if (keysPtr_->a) {
			camPhys.velocity.x = -NOCLIP_SPEED / sprintDiv;
		}
		if (keysPtr_->d) {
			camPhys.velocity.x = NOCLIP_SPEED / sprintDiv;
		}
		// Y DIR
		if (keysPtr_->space == keysPtr_->ctrl) {
			camPhys.velocity.y = 0;
		}
		if (keysPtr_->ctrl) {
			camPhys.velocity.y = -NOCLIP_SPEED / sprintDiv;
		}
		if (keysPtr_->space) {
			camPhys.velocity.y = NOCLIP_SPEED / sprintDiv;
		}
	}
	else { // GRAVITY
		// Z DIRECTION
		if (keysPtr_->w == keysPtr_->s) {
			camPhys.velocity.z = 0;
		}
		if (keysPtr_->w) {
			camPhys.velocity.z = -MAX_CAMERA_VELOCITY / sprintDiv;
		}
		if (keysPtr_->s) {
			camPhys.velocity.z = MAX_CAMERA_VELOCITY / sprintDiv;
		}
		// X DIRECTION
		if (keysPtr_->a == keysPtr_->d) {
			camPhys.velocity.x = 0;
		}
		if (keysPtr_->a) {
			camPhys.velocity.x = -MAX_CAMERA_VELOCITY / sprintDiv;
		}
		if (keysPtr_->d) {
			camPhys.velocity.x = MAX_CAMERA_VELOCITY / sprintDiv;
		}
		// Y DIR: JUMPING:
		if (!camPhys.grounded) {
			camPhys.velocity.y += GRAVITY_ACC * deltaT;
		}
		else {
			camPhys.velocity.y = 0;
		}
		if (camPhys.grounded && keysPtr_->space) {
			camPhys.jumping = true;
		}
		// FIXME HARDCODED SHIT, needs to be relative to grounded y position
		if (camPhys.jumping && camPhys.position.y < 2.2f) {
			camPhys.velocity.y += JUMP_ACC * deltaT;
		}
		if (camPhys.jumping && camPhys.position.y >= 2.2f) {
			camPhys.jumping = false;
		}
		/*if (keysPtr_->a == keysPtr_->d) {
			camPhys.velocity.x = 0;
		}
		if (keysPtr_->a) {
			camPhys.velocity.x = -MAX_CAMERA_VELOCITY / sprintDiv;
		}
		if (keysPtr_->d) {
			camPhys.velocity.x = MAX_CAMERA_VELOCITY / sprintDiv;
		}*/
	}
	
}
