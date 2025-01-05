#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "types.h"
#include "util.h"

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

class Dvce {
public:
  Dvce();
  ~Dvce();

  void setWindowPtr(SDL_Window *window);
  void init();

  // getters
  const VkPhysicalDevice &getPhysical() const;
  const VkDevice &getLogical() const;
  const VkSurfaceKHR &getSurface() const;
  const VkQueue &getGraphicsQue() const;
  const VkQueue &getPresentQue() const;
  SDL_Window *getWindowPtr() const;

  // wait idle
  void waitIdle() const;

  void cleanup();

private:
  // SDL WINDOW
  SDL_Window *window_ = nullptr;

  VkInstance instance_ = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger_ = VK_NULL_HANDLE;

  VkSurfaceKHR surface_ = VK_NULL_HANDLE;

  VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;

  VkQueue graphicsQueue_ = VK_NULL_HANDLE;
  VkQueue presentQueue_ = VK_NULL_HANDLE;

  // VK INSTANCE
  void createInstance();

  // VK DEBUG/VALIDATION
  void setupDebugMessenger();

  // SDL/VK SURFACE
  void createSurface();

  // VK PHYSCIAL DEVICE
  void selectPhysicalDevice();
  bool isDeviceSuitable(VkPhysicalDevice physicalDevice);

  // VK LOGICAL DEVICE
  void createLogicalDevice();
};