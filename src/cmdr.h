#pragma once

#include <vulkan/vulkan.h>

#include "util.h"
#include "dvce.h"

class Cmdr {
public:
	Cmdr();
	~Cmdr();

	// to give texture access to device
	void setDvcePtr(const Dvce& dvce);

	void createPool();

	// free to use by other code
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);


	void cleanup();

private:
	// reference to device:
	const Dvce* dvcePtr_ = nullptr;

	VkCommandPool commandPool_ = VK_NULL_HANDLE;

};