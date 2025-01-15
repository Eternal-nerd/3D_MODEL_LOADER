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
  window_ = SDL_CreateWindow("3D_MODEL_LOADER", WIDTH, HEIGHT, SDL_WINDOW_VULKAN);

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
    updateUBO();
    renderScene();

    // Limit FPS if wanted:
    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    // set another timepoint
    auto stopTime = std::chrono::high_resolution_clock::now();
    // get duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stopTime - startTime);
    double ms = duration.count() * 0.001; // convert to ms
    double fps = 1000 / ms;
    std::cout << "drawFrame() duration: " << ms << " milliseconds (" << fps << " FPS). \n";
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
void Engine::updateUBO() {
    gfx_.waitFrame();

    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    // DO MODEL TRANSFORMS
    for (int i = 0; i < renderables_.size(); i++) {
        ubo.model[i] = glm::mat4(1.0f);

        // move to model's position
        glm::vec3 pos = renderables_[i].position_;

        // can do fun stuff to positions here
        pos[1]*=sin(time+i);

        ubo.model[i] = glm::translate(ubo.model[i], pos);

        // rotate if wnated
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    // DO CAMERA TRANSFORMS
    VkExtent2D extent = gfx_.getSwapExtent();

    // LOOKAT(eyePos, centerPos (pointed at), up)
    ubo.view = glm::lookAt(glm::vec3(0, 20, 50), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(30.0f), extent.width / (float)extent.height, 0.1f, 100.0f);

    ubo.proj[1][1] *= -1;

    gfx_.mapUBO(ubo);
}

void Engine::generateRenderables() {
    util::log("Generating renderables... ");

    for (int i = 0; i < 400; i++) {
        RenderableData data;
        data.vertices = {
                { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },

                { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },

                { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },

                { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },

                { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },

                { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f } },
                { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f } },
                { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f } },
                { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f } },
        };

        data.indices = {
            0,1,2, 0,2,3, 6,5,4,  7,6,4, 10,9,8, 11,10,8, 12,13,14, 12,14,15, 18,17,16, 19,18,16, 20,21,22, 20,22,23
        };

        RenderableAccess access;
        gfx_.getRenderableAccess(access);

        Renderable r;

        r.init(i, data, access);

        r.position_ = { i%50-25, i%20-10, 0 };

        if (renderables_.size() < MAX_MODELS) {
            renderables_.push_back(r);
        }
        else {
            throw std::runtime_error("ATTEMPTING TO CREATE TOO MANY MODELS!  ");
        }
    }

}

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

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Engine::cleanup() {
  util::log("Cleaning up engine...");

  std::cout << "renderables_.size(): " << renderables_.size() << "\n";

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
