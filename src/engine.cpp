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

  util::log("Set Mouse into relative mode (for FPS camera)...");
  if (!SDL_SetWindowRelativeMouseMode(window_, true)) {
      throw std::runtime_error("Failed to put mouse into relative mode. ");
  }

  // Initialize gfx
  gfx_.setWindowPtr(window_);
  gfx_.init();

  // TODO: generate renderables here
  generateRenderables();

  // init camera
  VkExtent2D extent = gfx_.getSwapExtent();
  float aspect = extent.width / (float)extent.height;
  cam_.init(aspect);
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
    //auto startTime = std::chrono::high_resolution_clock::now();

    // FIXME the event handling system needs to run at a predefined rate, not just with the rate of the render loop...
    handleEvents();
    
    // only render stuff if window is visible
    if (visible_) {
        updateCamera();
        updateUBO();
        renderScene();
    }

    // Limit FPS if wanted:
    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    /*
    // set another timepoint
    auto stopTime = std::chrono::high_resolution_clock::now();
    // get duration
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        stopTime - startTime);
    double ms = duration.count() * 0.001; // convert to ms
    double fps = 1000 / ms;
    std::cout << "drawFrame() duration: " << ms << " milliseconds (" << fps << " FPS). \n";
    */
  }
  gfx_.deviceWaitIdle();
}

/*-----------------------------------------------------------------------------
------------------------------INPUT--------------------------------------------
-----------------------------------------------------------------------------*/
void Engine::handleEvents() {
  // Poll events (inputs)
  while (SDL_PollEvent(&event_)) {
    switch (event_.type) {
    // X OUT OF WINDOW
    case SDL_EVENT_QUIT:
        running_ = false;
        break;

    // Input EVENTS
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
    case SDL_EVENT_MOUSE_MOTION:
        handleInputEvent();
        break;

    // MINIMIZE/MAXIMIZE EVENTS
    case SDL_EVENT_WINDOW_HIDDEN:
    case SDL_EVENT_WINDOW_MINIMIZED:
        visible_ = false;
        break;
    case SDL_EVENT_WINDOW_SHOWN:
    case SDL_EVENT_WINDOW_MAXIMIZED:
    case SDL_EVENT_WINDOW_RESTORED:
        visible_ = true;
        break;
    default: break;
    }
  }
}

void Engine::handleInputEvent() {
    if (event_.key.scancode == SDL_SCANCODE_ESCAPE) { running_ = false; }
    
    if (event_.type != SDL_EVENT_MOUSE_MOTION) {
        // update keystates struct
        bool down = event_.type == SDL_EVENT_KEY_DOWN;
        switch (event_.key.scancode) {
        case SDL_SCANCODE_W:
            keys_.w = down;
            break;
        case SDL_SCANCODE_A:
            keys_.a = down;
            break;
        case SDL_SCANCODE_S:
            keys_.s = down;
            break;
        case SDL_SCANCODE_D:
            keys_.d = down;
            break;
        case SDL_SCANCODE_SPACE:
            keys_.space = down;
            break;
        case SDL_SCANCODE_LCTRL:
            keys_.ctrl = down;
            break;
        }
    }

    // FOR NOW input events only really affect the camera..
    cam_.processEvent(event_, keys_);
}


/*-----------------------------------------------------------------------------
-----------------------------UPDATE-STUFF--------------------------------------
-----------------------------------------------------------------------------*/
void Engine::updateCamera() {
    VkExtent2D extent = gfx_.getSwapExtent();
    float aspect = extent.width / (float)extent.height;
    cam_.update(aspect);
}

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
        // FIXME
        pos.y*=sin(time+i);
        pos.z = 5 * sin(time + i/2);

        ubo.model[i] = glm::translate(ubo.model[i], pos);

        // rotate if wnated
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
     }
    
    ubo.view = cam_.getViewProj();
    ubo.proj = cam_.getPerspectiveProj();

    gfx_.mapUBO(ubo);
}

void Engine::generateRenderables() {
    util::log("Generating renderables... ");

    //FIXME
    //for (int i = 0; i < 2; i++) {
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

        // FIXME
        //r.position_ = { 2*(i-0.5), 0, 0 };
        r.position_ = { i % 50 - 25, i % 20 - 10, 0 };

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
