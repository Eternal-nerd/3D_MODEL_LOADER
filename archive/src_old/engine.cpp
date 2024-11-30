#include "engine.h"

#include <iostream>

Engine::Engine() { init(); }

Engine::~Engine() { cleanup(); }

void Engine::init() {
  std::cout << "init engine \n";
  // init SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Failed to initialize SDL. ");
  }
  win_ = SDL_CreateWindow("3D_MODEL_LOADER", 800, 600, SDL_WINDOW_VULKAN);
  if (!win_) {
    throw std::runtime_error("Failed to create SDL window. ");
  }

  if (!SDL_SetWindowResizable(win_, true)) {
    throw std::runtime_error("Failed to make window resizable. ");
  }

  // Init Vulkan
  // create instance:
  // params_.makeInstanceCreateInfo();
  if (vkCreateInstance(&params_.instanceCreateInfo, nullptr, &instance_) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

void Engine::run() {
  std::cout << "run engine \n";

  running_ = true;
  while (running_) {
    // Poll events
    while (SDL_PollEvent(&event_)) {
      if (event_.type == SDL_EVENT_QUIT) {
        running_ = false;
      }
    }
    // draw frame
    draw_frame();
  }
  // vkDeviceWaitIdle(device_);
}

void Engine::draw_frame() {}

void Engine::cleanup() {
  std::cout << "cleanup engine \n";

  // SDL Last
  SDL_DestroyWindow(win_);

  SDL_Quit();
}

void Engine::debug_test() {
  std::cout << "testing a function. \n";

  std::cout << "End of debug_test(). \n";
}
