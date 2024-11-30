#pragma once

#include <vector>
#include <vulkan/vulkan.h>

// FIXME, maybe config stucts dont need to be stored in class... since theyre
// one time use (I think?)
class vk_params {
public:
  vk_params();
  ~vk_params();

  // instance creation
  void makeInstanceCreateInfo();
  VkApplicationInfo appInfo{};
  VkInstanceCreateInfo instanceCreateInfo{};

private:
  std::vector<const char *> getRequiredExtensions();
};
