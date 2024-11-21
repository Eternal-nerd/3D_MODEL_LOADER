#include <cstdlib>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "engine.h"

// Global variables
SDL_Window *window;
Engine *engine;

int run() {
  // init SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Failed to initialize SDL. ");
  }
  window = SDL_CreateWindow("3D_MODEL_LOADER", 800, 600, SDL_WINDOW_VULKAN);
  if (!window) {
    throw std::runtime_error("Failed to create SDL window. ");
  }

  if (!SDL_SetWindowResizable(window, true)) {
    throw std::runtime_error("Failed to make window resizable. ");
  }

  engine = new Engine(window); // historically: vulkan = new Vulkan();
  // engine->init();        // historically: init_vulkan_extern(vulkan);

  SDL_Event event;
  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    // engine->AcquireNextImage();

    // engine->ResetCommandBuffer();
    // engine->BeginCommandBuffer();
    {
      // VkClearColorValue clear_color = {1.0f, 0.0f, 0.0f, 1.0f};
      // VkClearDepthStencilValue clear_depth_stencil = {1.0f, 0};
      // engine->BeginRenderPass(clear_color,clear_depth_stencil);
      {}
      // EndRenderPass();
    }
    // EndCommandBuffer();

    // QueueSubmit();
    // QueuePresent();
  }

  delete engine;
  engine = nullptr;

  SDL_DestroyWindow(window);
  window = nullptr;

  SDL_Quit();
  return 0;
}

int main() {
  try {
    std::cout << "HELLO WORLT. ;)\n";
    run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
