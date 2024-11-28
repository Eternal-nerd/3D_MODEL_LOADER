#include "vk_params.h"

#include <SDL3/SDL_vulkan.h>

// remove/use default?
vk_params::vk_params() {}
vk_params::~vk_params() {}

VkInstanceCreateInfo vk_params::getInstanceCreateInfo() {
  // APP Info here
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "3D_MODEL_LOADER";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  // get extensions from SDL
  auto extensions = getRequiredExtensions();
  instanceCreateInfo.enabledExtensionCount =
      static_cast<uint32_t>(extensions.size());
  instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

  return instanceCreateInfo;
}

// Utility functions:
std::vector<const char *> vk_params::getRequiredExtensions() {
  // variable holding extensions to return:
  std::vector<const char *> extensions;

  uint32_t sdl_extension_count = 0;
  const char *const *sdl_extensions =
      SDL_Vulkan_GetInstanceExtensions(&sdl_extension_count);
  for (uint32_t i = 0; i < sdl_extension_count; i++) {
    extensions.push_back(sdl_extensions[i]);
  }

  // FIXME add??
  /*if (enableValidationLayers) {
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
}*/

  return extensions;
}
