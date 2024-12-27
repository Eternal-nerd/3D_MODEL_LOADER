#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

// FIXME Minimal includes PLEASE
#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <optional>
#include <vector>

// My useless abstractions
#include "data.h"
#include "renderer.h"
#include "types.h"
#include "util.h"

class Engine {
public:
  void run();

private:
  // SDL Shit
  SDL_Window *window_ = nullptr;
  SDL_Event event_;
  bool running_ = false;

  // Renderer (Vulkan encapsulated)
  Renderer renderer_;

  /* BIG PICTURE METHODS */
  void collectInputs(); // get user input data to update camera/scene
  void updateCamera();  // update camera based on inputs
  void updateScene();   // perform transformations to objects being rendered
  void renderScene();   // GPU renders scene image
  // TODO after effects (applied to rendered image)
  void
  swapBuffers(); // Presents rendered image to user, maybe rename function TODO

  void init();
  void mainLoop();
  void cleanup();

};
