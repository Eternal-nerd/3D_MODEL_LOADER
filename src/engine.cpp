#include "engine.h"

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
    util::log("Initializing clock...");
    clock_.init();

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
  gfx_.init(window_);

  // generate meshs
  generateRenderables();

  // init camera
  VkExtent2D extent = gfx_.getSwapExtent();
  float aspect = extent.width / (float)extent.height;
  cam_.init(aspect, clock_);
}

/*-----------------------------------------------------------------------------
------------------------------MAIN-LOOP----------------------------------------
-----------------------------------------------------------------------------*/

void Engine::renderLoop() {
  util::log("Begining render loop...");

  running_ = true;
  while (running_) {
    // measure frametime
    clock_.startFrame();
    fpsCounter_++;

    // FIXME the event handling system needs to run at a predefined rate, not just with the rate of the render loop...
    handleEvents();
    
    // only render stuff if window is visible
    if (visible_) {
        updateCamera();
        updateUBO();
        renderScene();
        drawUI();
        presentImage();
    }

    // TODO find FPS and display as onscreen text
    // for now, report fps every 100 frames
    hundredFrameTime_ += clock_.getFrameDelta();
    if (fpsCounter_ >= 99) {
        // get average frame time Ns: 
        long avg = hundredFrameTime_ / fpsCounter_;
        
        // print fps
        std::cout << "FPS: " << 1000000000 / avg << "\n";

        fpsCounter_ = 0;
        hundredFrameTime_ = 0;
    }
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
    float time = clock_.getProgramSeconds();

    UniformBufferObject ubo{};
    // DO MODEL TRANSFORMS
    for (int i = 0; i < renderables_.size(); i++) {
        ubo.model[i] = glm::mat4(1.0f);

        // move to model's position
        glm::vec3 pos = renderables_[i].position_;

        // can do fun stuff to positions here

        ubo.model[i] = glm::translate(ubo.model[i], pos);

        // rotate if wnated
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
     }
    
    ubo.view = cam_.getViewProj();
    ubo.proj = cam_.getPerspectiveProj();

    // wait for frame to be ready
    gfx_.waitFrame();

    gfx_.mapUBO(ubo);
}

void Engine::generateRenderables() {
    util::log("Generating renderables... ");

    RenderableAccess access;
    gfx_.getRenderableAccess(access);

    // cube
    Cube cubeData;
    RenderableData data;
    data.vertices = cubeData.vertices;
    data.indices = cubeData.indices;
    Renderable r1;
    r1.initSimple(0, data, access);
    r1.position_ = { -2,0,0 };
    r1.setTextureIndex(2);
    renderables_.push_back(r1);

    // gltf??
    std::string filename = "../res/gltf/Fox-glTF-Sample-Models/glTF-Binary/Fox.glb";
    Renderable r2;
    r2.initGLTF(1, access, filename);
    r2.position_ = { 2,0,0 };
    // FIXMEMMEE
    //renderables_.push_back(r2);

    if (renderables_.size() >= MAX_MODELS) {
        throw std::runtime_error("ATTEMPTING TO CREATE TOO MANY MODELS!  ");
    }
}

/*-----------------------------------------------------------------------------
------------------------------GFX-RENDERING------------------------------------
-----------------------------------------------------------------------------*/
void Engine::renderScene() {
    VkCommandBuffer commandBuffer;

    commandBuffer = gfx_.setupCommandBuffer();

    // draw renderables
    for (auto renderable : renderables_) {
        renderable.bind(commandBuffer);
        renderable.draw(commandBuffer);
    }

    gfx_.submitCommandBuffer(commandBuffer);
}

void Engine::presentImage() {
    gfx_.presentSwapchainImage();
}


/*-----------------------------------------------------------------------------
-----------------------------TESTING-HUD---------------------------------------
-----------------------------------------------------------------------------*/
void Engine::drawUI() {
    gfx_.drawUI();
}

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
