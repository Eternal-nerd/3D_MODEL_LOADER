#include "engine.h"

#include <stdexcept>

// my abstractions
#include "util.h"
#include "data.h"

Engine::Engine() {}

Engine::~Engine() {}

void Engine::run() {
	util::log("Running engine...");
	init();
	renderLoop();
	cleanup();
}

void Engine::init() {
	util::log("Initializing SDL...");
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw std::runtime_error("Failed to initialize SDL. ");
	}

	util::log("Creating SDL window...");
	window_ =
		SDL_CreateWindow("3D_MODEL_LOADER", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);

	if (!window_) {
		throw std::runtime_error("Failed to create SDL window. ");
	}

	util::log("Setting SDL window to be resizable...");
	if (!SDL_SetWindowResizable(window_, true)) {
		throw std::runtime_error("Failed to make window resizable. ");
	}
}

void Engine::renderLoop() {
	running_ = true;
	while (running_) {
		collectInputs();
		updateCamera();
		updateScene();
		renderScene();
		swapBuffers();
	}
}

void Engine::collectInputs() {
	// Poll events (inputs)
	while (SDL_PollEvent(&event_)) {
		if (event_.type == SDL_EVENT_QUIT) {
			running_ = false;
		}
		else {
			// TODO switch statement of all possible inputs?
		}

	}
}

void Engine::updateCamera() {}

void Engine::updateScene() {}

void Engine::renderScene() {

}

void Engine::swapBuffers() {}

void Engine::cleanup() {}