#include "gfx.h"

Gfx::Gfx() {}
Gfx::~Gfx() {}

void Gfx::setWindowPtr(SDL_Window* window) { window_ = window; }

void Gfx::init() {
	util::log("Initializing Gfx...");
	
	// initialize dvce
	dvce_.setWindowPtr(window_);
	dvce_.init();
}

void Gfx::drawRenderable() {}

void Gfx::cleanup() {
	util::log("Cleaning up Gfx...");

	/* cleanup dvce
	*/
	dvce_.cleanup();
}