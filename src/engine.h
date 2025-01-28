#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>

// my abstractions
#include "util.h"
#include "gfx.h"
#include "renderable.h"
#include "camera.h"
#include "clock.h"
#include "types.h"

const uint32_t WIDTH = 1600;
const uint32_t HEIGHT = 800;

class Engine {
public:
  Engine();
  ~Engine();

  void run();

private:
	// time
	Clock clock_;

	int fpsCounter_ = 0;
	long hundredFrameTime_ = 0;

  // SDL Shit
  SDL_Window* window_ = nullptr;
  SDL_Event event_;
  bool running_ = false;
  bool visible_ = true;

  // Gfx (Vulkan), used to draw Renderable objects
  Gfx gfx_;

  std::vector<Renderable> renderables_ = {};

  KeyStates keys_;

  Camera cam_;

  // initializes the application
  void init();

  void generateRenderables();

  // loop that executes for each frame
  void renderLoop();
  
  void handleEvents(); // get user input data to update camera/scene
  void handleInputEvent();

  void updateCamera();

  void updateUBO(); //
  void renderScene(); // GPU renders scene image, draw call for each model

  void drawUI();
  void presentImage();

  // cleans up the application on termination
  void cleanup();
};
