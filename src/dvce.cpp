#include "dvce.h"

#include <stdexcept>

Dvce::Dvce() {}
Dvce::~Dvce() {}

void Dvce::setWindowPtr(SDL_Window* window) { window_ = window; }

void Dvce::init() {
	util::log("Initializing Device...");

	createInstance();
	setupDebugMessenger();
	createSurface();
	//pickPhysicalDevice();
	//createLogicalDevice();
}

void Dvce::createInstance() {
    util::log("Creating Vulkan instance...");

    if (enableValidationLayers && !util::checkValidationLayerSupport(validationLayers)) {
        throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "3D_MODEL_LOADER";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = util::getRequiredExtensions();
    if (enableValidationLayers) {
        // need to add debug util to extensions
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount =
            static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        util::populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void Dvce::setupDebugMessenger() {
    if (!enableValidationLayers)
        return;

    util::log("Setting up debug messenger...");
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    util::populateDebugMessengerCreateInfo(createInfo);

    if (util::CreateDebugUtilsMessengerEXT(instance_, &createInfo, nullptr,
        &debugMessenger_) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

void Dvce::createSurface() {
    util::log("Creating surface...");
    if (!SDL_Vulkan_CreateSurface(window_, instance_, nullptr, &surface_)) {
        throw std::runtime_error("failed to create SDL window surface!");
    }
}

void Dvce::cleanup() {
	util::log("Cleaning up Device...");




    util::log("Destroying Debug Messenger...");
    if (enableValidationLayers) {
        util::DestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
    }

    util::log("Destroying Vulkan instance...");
    vkDestroyInstance(instance_, nullptr);
}