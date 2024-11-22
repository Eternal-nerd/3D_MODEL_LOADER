#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

class Engine {
public:
  Engine(SDL_Window *win);
  ~Engine();

  void run();

private:
  SDL_Window *win_;

  void init();
};
