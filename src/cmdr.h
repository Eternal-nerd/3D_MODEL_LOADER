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
  void setDvcePtr(const Dvce &dvce);

  void createCommandPool();
  void createCommandBuffers(int maxFramesInFlight);

  // free to use by other code
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);

  // getters
  const VkCommandPool &getCommandPool() const;
  const std::vector<VkCommandBuffer>& getCommandBuffers() const;

  void cleanup();

private:
  // reference to device:
  const Dvce *dvcePtr_ = nullptr;

  VkCommandPool commandPool_ = VK_NULL_HANDLE;
  std::vector<VkCommandBuffer> commandBuffers_;
};