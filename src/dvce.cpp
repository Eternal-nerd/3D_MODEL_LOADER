#include "dvce.h"

#include <stdexcept>

Dvce::Dvce() {}
Dvce::~Dvce() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/

void Dvce::setWindowPtr(SDL_Window *window) { window_ = window; }

void Dvce::init() {
  util::log("Initializing Device...");

  createInstance();
  setupDebugMessenger();
  createSurface();
  selectPhysicalDevice();
  createLogicalDevice();
}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/

const VkPhysicalDevice &Dvce::getPhysical() const { return physicalDevice_; }
const VkDevice &Dvce::getLogical() const { return device_; }
const VkSurfaceKHR &Dvce::getSurface() const { return surface_; }
const VkQueue &Dvce::getGraphicsQue() const { return graphicsQueue_; }
const VkQueue &Dvce::getPresentQue() const { return presentQueue_; }
SDL_Window *Dvce::getWindowPtr() const { return window_; }

/*-----------------------------------------------------------------------------
------------------------------VK-INSTANCE--------------------------------------
-----------------------------------------------------------------------------*/

void Dvce::createInstance() {
  util::log("Creating Vulkan instance...");

  if (enableValidationLayers &&
      !util::checkValidationLayerSupport(validationLayers)) {
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
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;

    createInfo.pNext = nullptr;
  }

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create instance!");
  }
}

/*-----------------------------------------------------------------------------
-----------------------------DEBUG-MESSENGER-----------------------------------
-----------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------
-----------------------------VK-SDL-SURFACE------------------------------------
-----------------------------------------------------------------------------*/

void Dvce::createSurface() {
  util::log("Creating surface...");
  if (!SDL_Vulkan_CreateSurface(window_, instance_, nullptr, &surface_)) {
    throw std::runtime_error("failed to create SDL window surface!");
  }
}

/*-----------------------------------------------------------------------------
------------------------------PHYSICAL-DEVICE----------------------------------
-----------------------------------------------------------------------------*/

void Dvce::selectPhysicalDevice() {
  util::log("Selecting physical device...");
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

  for (const auto &device : devices) {
    if (isDeviceSuitable(device)) {
      physicalDevice_ = device;
      break;
    }
  }

  if (physicalDevice_ == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }
}

bool Dvce::isDeviceSuitable(VkPhysicalDevice physicalDevice) {
  QueueFamilyIndices indices =
      util::findQueueFamilies(physicalDevice, surface_);

  bool extensionsSupported =
      util::checkDeviceExtensionSupport(physicalDevice, deviceExtensions);

  bool swapChainAdequate = false;
  if (extensionsSupported) {
    SwapChainSupportDetails swapChainSupport =
        util::querySwapChainSupport(physicalDevice, surface_);
    swapChainAdequate = !swapChainSupport.formats.empty() &&
                        !swapChainSupport.presentModes.empty();
  }

  VkPhysicalDeviceFeatures supportedFeatures;
  vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

  return indices.isComplete() && extensionsSupported && swapChainAdequate &&
         supportedFeatures.samplerAnisotropy;
}

/*-----------------------------------------------------------------------------
------------------------------LOGICAL-DEVICE-----------------------------------
-----------------------------------------------------------------------------*/

void Dvce::createLogicalDevice() {
  util::log("Creating logical device...");
  QueueFamilyIndices indices =
      util::findQueueFamilies(physicalDevice_, surface_);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                            indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();

  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(deviceExtensions.size());
  createInfo.ppEnabledExtensionNames = deviceExtensions.data();

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
  vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
}

void Dvce::waitIdle() const { vkDeviceWaitIdle(device_); }

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/

void Dvce::cleanup() {
  util::log("Cleaning up Device...");

  util::log("Destroying logical device...");
  vkDestroyDevice(device_, nullptr);

  util::log("Destroying surface...");
  vkDestroySurfaceKHR(instance_, surface_, nullptr);

  util::log("Destroying Debug Messenger...");
  if (enableValidationLayers) {
    util::DestroyDebugUtilsMessengerEXT(instance_, debugMessenger_, nullptr);
  }

  util::log("Destroying Vulkan instance...");
  vkDestroyInstance(instance_, nullptr);
}