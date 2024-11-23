#pragma once

#include <vulkan/vulkan.h>

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
