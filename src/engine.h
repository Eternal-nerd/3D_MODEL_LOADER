#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

#include "input_structs.h"
#include "util_structs.h"

class Engine {
public:
  Engine();
  ~Engine();

  void run();

private:
  // SDL Shit
  SDL_Window *win_ = nullptr;
  SDL_Event event_;
  bool running_ = false;

  // Vulkan Shit
  VkInstance instance;
  VkDebugUtilsMessengerEXT debugMessenger;
  VkSurfaceKHR surface;
  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  VkDevice device;

  void init();
  void draw_frame();
  void cleanup();
};
