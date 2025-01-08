#include "engine.h"

#include <chrono>
#include <iostream>
#include <stdexcept>
#include <thread>

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

  // TODO: generate renderables here
  generateRenderables();
}

void Engine::generateRenderables() {
  util::log("Generating renderables... ");

  //for (int i = 0; i < 5; i++) {

    RenderableData data;
    data.vertices = {
    {{-0.5, -0.5, 0.5}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, 
    {{0.5, -0.5, 0.5}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5, 0.5, 0.5}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5, 0.5, 0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5, -0.5, -0.5}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5, -0.5, -0.5}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5, 0.5, -0.5}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5, 0.5, -0.5}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}} };

    data.indices = {
    0, 1, 2, 2, 3, 0, // top face
    6, 5, 4, 4, 7, 6, // bottom face
    4, 1, 0, 4, 5, 1, 
    5, 2, 1, 5, 6, 2,
    6, 3, 2, 6, 7, 3,
    7, 4, 3, 4, 0, 3
    };

    RenderableAccess access;
    gfx_.getRenderableAccess(access);

    Renderable r;

    r.init(data, access);

    renderables_.push_back(r);
  //}

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
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // set another timepoint
    auto stopTime = std::chrono::high_resolution_clock::now();
    // get duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stopTime - startTime);
    double ms = duration.count() * 0.001; // convert to ms
    double fps = 1000 / ms;
    std::cout << "drawFrame() duration: " << ms << " milliseconds (" << fps
              << " FPS). \n";
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
    } else {
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
    VkCommandBuffer commandBuffer;

    commandBuffer = gfx_.beginFrame();

    // draw renderables
    for (auto renderable : renderables_) {
        renderable.bind(commandBuffer);
        renderable.draw(commandBuffer);
    }

    gfx_.endFrame(commandBuffer);

}

void Engine::swapBuffers() {}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Engine::cleanup() {
  util::log("Cleaning up engine...");

  // cleanup gfx
  gfx_.cleanupStart();

  for (auto r : renderables_) {
      r.cleanup();
  }

  gfx_.cleanupEnd();

  util::log("Cleaning up SDL...");
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
