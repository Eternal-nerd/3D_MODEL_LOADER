#include "vk_params.h"

// remove/use default?
vk_params::vk_params() {}
vk_params::~vk_params() {}

VkApplicationInfo vk_params::getAppInfo() {
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "3D_MODEL_LOADER";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;
  return appInfo;
}

VkInstanceCreateInfo vk_params::getInstanceCreateInfo() {
  // instanceCreateInfo.
  return instanceCreateInfo;
}
