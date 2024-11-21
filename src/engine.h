#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

class Engine {
public:
  Engine(SDL_Window *win);
  ~Engine();

private:
  SDL_Window *win_;

  void init();
};
