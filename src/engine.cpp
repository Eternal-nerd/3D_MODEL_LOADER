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

    phys_.init(clock_);

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

    GfxAccess access;
    gfx_.getAccess(access);

    // initialize text overlay
    text_.init(access);
    updateTextOverlay();

    // generate meshs
    generateRenderables(access);

    // init camera
    VkExtent2D extent = gfx_.getSwapExtent();
    float aspect = extent.width / (float)extent.height;
    cam_.init(aspect, clock_, phys_);
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

    // get user inputs
    handleEvents();
    
    // only render stuff if window is visible
    if (visible_) {
        updateCamera();
        updateUBO();
        updateTextOverlay();
        renderScene();
        // FIXME Text overlay update?  
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
        case SDL_SCANCODE_N:
            keys_.n = down;
            break;
        }
    }

    // FOR NOW mouse input events only really affect the camera..
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
        ubo.model[i] = glm::translate(ubo.model[i], pos);

        // i=id
        switch (i) {
        case 0:
            // TODO Skybox
            ubo.model[i] = glm::scale(ubo.model[i], { 1000.f, 1000.f, 1000.f });
            break;
        case 1:
            // FLOOR
            ubo.model[i] = glm::scale(ubo.model[i], { 100.f, 0.f, 100.f });
            ubo.model[i] = glm::rotate(ubo.model[i], glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 2:
            // CUBE
            ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.model[i] = glm::rotate(ubo.model[i], time * glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case 3:
            // VIKING ROOM
            ubo.model[i] = glm::scale(ubo.model[i], { 5.f, 5.f, 5.f });
            ubo.model[i] = glm::rotate(ubo.model[i], glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            ubo.model[i] = glm::rotate(ubo.model[i], glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case 4:
            // HOUSE
            ubo.model[i] = glm::scale(ubo.model[i], { 0.1f, 0.1f, 0.1f });
            break;
        default:
            break;
        }
    }
    
    ubo.view = cam_.getViewProj();
    ubo.proj = cam_.getPerspectiveProj();

    // wait for frame to be ready
    gfx_.waitFrame();

    gfx_.mapUBO(ubo);
}

void Engine::generateRenderables(const GfxAccess& access) {
    util::log("Generating renderables... ");

    // premades
    InvertedCube invertedCubeData;
    Cube cubeData;
    Plane planeData;
    LetterQuad letterQuad;

    // Skybox
    Renderable skybox;
    RenderableData skyboxData;
    skyboxData.vertices = invertedCubeData.vertices;
    skyboxData.indices = invertedCubeData.indices;
    skybox.init(0, skyboxData, access);
    skybox.position_ = { 0,0,0 };
    setRenderableTextureIndex(skybox, 0);
    renderables_.push_back(skybox);

    // FLOOR
    Renderable floor;
    RenderableData floorData;
    floorData.vertices = planeData.vertices;
    floorData.indices = planeData.indices;
    floor.init(1, floorData, access);
    floor.position_ = { 0,0,0 };
    setRenderableTextureIndex(floor, 4);
    renderables_.push_back(floor);

    // cube
    RenderableData data1;
    data1.vertices = cubeData.vertices;
    data1.indices = cubeData.indices;
    Renderable r1;
    r1.init(2, data1, access);
    r1.position_ = { -2,2,0 };
    setRenderableTextureIndex(r1, 2);
    renderables_.push_back(r1);

    // viking room model
    Renderable r2;
    r2.init(3, util::getObjData("../res/obj/viking_room.obj"), access);
    r2.position_ = {20, 0, 0};
    setRenderableTextureIndex(r2, 1);
    renderables_.push_back(r2);

    // house
    Renderable r3;
    r3.init(4, util::getObjData("../res/obj/house.obj"), access);
    r3.position_ = { -30, 0, 0 };
    setRenderableTextureIndex(r3, 3);
    renderables_.push_back(r3);

    //FIXME
    Renderable font;
    RenderableData fontData;
    fontData.vertices = planeData.vertices;
    fontData.indices = planeData.indices;
    font.init(5, fontData, access);
    font.position_ = { 0,2,-5 };
    setRenderableTextureIndex(font, 5);
    renderables_.push_back(font);
    // test letter
    /*Renderable letter;
    RenderableData letterData;
    letterData.vertices = letterQuad.vertices;
    // mess with texture cords here
    //letterData.vertices[0].texCoord = { 0.0f, 1.0f };
    //letterData.vertices[1].texCoord = { 1.0f, 1.0f };
    //letterData.vertices[2].texCoord = { 1.0f, 0.0f };
    //letterData.vertices[3].texCoord = { 0.0f, 0.0f };

    letterData.indices = letterQuad.indices;
    letter.init(4, letterData, access);
    letter.position_ = {0, 2, 0};
    setRenderableTextureIndex(letter, 0);
    renderables_.push_back(letter);*/

    if (renderables_.size() >= MAX_MODELS) {
        throw std::runtime_error("ATTEMPTING TO CREATE TOO MANY MODELS!  ");
    }
}

void Engine::setRenderableTextureIndex(Renderable& renderable, int index) {
    if (index > gfx_.getTextureCount()) {
        throw std::runtime_error("TEXTURE INDEX OUT OF BOUNDS");
    }

    renderable.setTextureIndex(index);
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

    text_.draw(commandBuffer);

    gfx_.submitCommandBuffer(commandBuffer);
}

void Engine::presentImage() {
    gfx_.presentSwapchainImage();
}


/*-----------------------------------------------------------------------------
-----------------------------TESTING-HUD---------------------------------------
-----------------------------------------------------------------------------*/
void Engine::updateTextOverlay() {
    // Update the text buffer displayed by the text overlay
    //uint32_t lastNumLetters = text_.numLetters; // FIXME REMOVE?

    text_.beginTextUpdate();

    text_.addText("Hello World. ", -1.f, -0.9f);

    text_.endTextUpdate();
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

  // cleanup text overlay shit
  text_.cleanup();

  gfx_.cleanupEnd();

  util::log("Cleaning up SDL...");
  SDL_DestroyWindow(window_);
  SDL_Quit();
}
