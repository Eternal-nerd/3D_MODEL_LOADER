#pragma once

#include <vulkan/vulkan.h>

class vk_params {
public:
  // constructor (populate structs)
  vk_params();
	~vk_params();

	void initAppInfo();
	VkApplicationInfo appInfo{};
	VkInstanceCreateInfo instanceCreateInfo{};
  

private:
	// UTIL methods

};
