#pragma once

#include <vector>
#include <vulkan/vulkan.h>

// FIXME, maybe config stucts dont need to be stored in class... since theyre
// one time use (I think?)
class vk_params {
public:
  vk_params();
  ~vk_params();

  // VkApplicationInfo getAppInfo();
  VkInstanceCreateInfo getInstanceCreateInfo();

private:
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo instanceCreateInfo{};

  std::vector<const char *> getRequiredExtensions();
};
