#include "engine.h"

#include <stdexcept>
#include <chrono>
#include <iostream>
#include <thread>

// my abstractions
#include "util.h"

Engine::Engine() {}

Engine::~Engine() {}

/*-----------------------------------------------------------------------------
-----------------------------ONLY-PUBLIC-METHOD--------------------------------
-----------------------------------------------------------------------------*/

void Engine::run() {
	util::log("Running engine...");
	init();
	renderLoop();
	cleanup();
}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/

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

	// Initialize gfx
	gfx_.setWindowPtr(window_);
	gfx_.init();
}

/*-----------------------------------------------------------------------------
------------------------------MAIN-LOOP----------------------------------------
-----------------------------------------------------------------------------*/

void Engine::renderLoop() {
	util::log("Begining render loop...");

	running_ = true;
	while (running_) {
		// measure frametime
		// set a timepoint
		auto startTime = std::chrono::high_resolution_clock::now();

		collectInputs();
		updateCamera();
		updateScene();
		renderScene();
		swapBuffers();
	
		// Limit FPS if wanted:
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// set another timepoint
		auto stopTime = std::chrono::high_resolution_clock::now();
		// get duration
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime);
		double ms = duration.count() * 0.001; // convert to ms
		double fps = 1000 / ms;
		std::cout << "drawFrame() duration: " << ms << " milliseconds (" << fps << " FPS). \n";
	}
	gfx_.deviceWaitIdle();
}

/*-----------------------------------------------------------------------------
------------------------------INPUT--------------------------------------------
-----------------------------------------------------------------------------*/
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

/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Engine::updateCamera() {}

void Engine::updateScene() {}

/*-----------------------------------------------------------------------------
------------------------------GFX-RENDERING------------------------------------
-----------------------------------------------------------------------------*/
void Engine::renderScene() {
	// device wait idle here?
	gfx_.tempDrawFrame();
}

void Engine::swapBuffers() {}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Engine::cleanup() {
	util::log("Cleaning up engine...");

	// cleanup gfx
	gfx_.cleanup();

	util::log("Cleaning up SDL...");
	SDL_DestroyWindow(window_);
	SDL_Quit();
}