#include "engine.h"

#include <thread>

// ENGINE IMPLEMENTATION
void Engine::run() {
  init();
  mainLoop();
  cleanup();
}

void Engine::init() {
  // init SDL
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error("Failed to initialize SDL. ");
  }

  window_ =
      SDL_CreateWindow("3D_MODEL_LOADER", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);

  if (!window_) {
    throw std::runtime_error("Failed to create SDL window. ");
  }

  if (!SDL_SetWindowResizable(window_, true)) {
    throw std::runtime_error("Failed to make window resizable. ");
  }

  renderer_.setWindowPtr(window_);
  renderer_.init();
}

void Engine::mainLoop() {
  std::cout << "run engine \n";

  running_ = true;
  while (running_) {
    // Poll events
    while (SDL_PollEvent(&event_)) {
      if (event_.type == SDL_EVENT_QUIT) {
        running_ = false;
      } else {
        collectInputs();
      }
    }

    // measure frametime
    // set a timepoint
    auto startTime = std::chrono::high_resolution_clock::now();

    renderer_.drawFrame();

    // Limit FPS if wanted:
    std::this_thread::sleep_for(std::chrono::milliseconds(60));

    // set another timepoint
    auto stopTime = std::chrono::high_resolution_clock::now();
    // get duration of drawFrame() method
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stopTime - startTime);
    double ms = duration.count() * 0.001; // convert to ms
    double fps = 1000 / ms;
    std::cout << "drawFrame() duration: " << ms << " milliseconds (" << fps
              << " FPS). \n";
  }

  renderer_.deviceWaitIdle();
}

/* BIG PICTURE METHODS
 */
void Engine::collectInputs() {
  // TODO add inputs
  // switch () {

  //}
}
void Engine::updateCamera() {
  // TODO Update UBO here?
}
void Engine::updateScene() {}
void Engine::renderScene() {}
// TODO after effects (applied to rendered image)
void Engine::swapBuffers() {}

void Engine::cleanup() {
  renderer_.cleanup();

  // SDL Cleanup
  SDL_DestroyWindow(window_);
  SDL_Quit();
}

/*
// TODO: this is where model is moved and shit
void Engine::updateUniformBuffer(uint32_t currentImage) {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};
  // DO ROTATION
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  // ubo.model *= glm::rotate(glm::mat4(1.0f), time * glm::radians(1750.0f),
  //                          glm::vec3(0.0f, 1.0f, 0.0f));

  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(glm::radians(30.0f),
                              renderer_.swapChainExtent_.width /
                                  (float)renderer_.swapChainExtent_.height,
                              0.1f, 10.0f);
  ubo.proj[1][1] *= -1;

  memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}
*/
