#pragma once

#include <vulkan/vulkan.h>

// IDEA: create this obejct, then call getters to
// initialize and return the appropriate VK API input
// struct.  Can pass external needed data in only when
// needed. Ex: output from SDL_Vulkan_GetInstanceExtensions()
class vk_params {
public:
  vk_params();
  ~vk_params();

  VkApplicationInfo getAppInfo();
  VkInstanceCreateInfo getInstanceCreateInfo();

private:
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo instanceCreateInfo{};
};
