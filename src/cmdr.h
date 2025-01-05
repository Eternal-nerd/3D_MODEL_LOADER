#pragma once

#include <vulkan/vulkan.h>

#include "dvce.h"
#include "util.h"

class Dvce;

class Cmdr {
public:
	Cmdr();
	~Cmdr();

	// to give cmdr access to device
	void setDvcePtr(const Dvce& dvce);

	void createPool();

	// free to use by other code
	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	// getter
	const VkCommandPool& getCommandPool() const;

	void cleanup();

private:
	// reference to device:
	const Dvce* dvcePtr_ = nullptr;

	VkCommandPool commandPool_ = VK_NULL_HANDLE;

};