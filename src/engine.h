#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "gfx.h"

const uint32_t WIDTH = 300;
const uint32_t HEIGHT = 200;

class Engine {
public:
	Engine();
	~Engine();

	void run();

private:
	// SDL Shit
	SDL_Window* window_ = nullptr;
	SDL_Event event_;
	bool running_ = false;

	// Gfx (Vulkan), used to draw Renderable objects
	Gfx gfx_;

	// initializes the application
	void init();

	// loop that executes for each frame
	void renderLoop();
	// methods inside the render loop:
	void collectInputs(); // get user input data to update camera/scene
	void updateCamera();  // update camera based on inputs
	void updateScene();   // perform transformations to objects/models being rendered
	void renderScene();   // GPU renders scene image, draw call for each model
	void swapBuffers();  // swaps frame currently in view with back buffered image? 
	
	// cleans up the application on termination
	void cleanup();


};

