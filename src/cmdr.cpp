#include "cmdr.h"

#include <stdexcept>

Cmdr::Cmdr() {}
Cmdr::~Cmdr() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Cmdr::setDvcePtr(const Dvce &dvce) { dvcePtr_ = &dvce; }

void Cmdr::createCommandPool() {
  util::log("Creating command pool...");
  QueueFamilyIndices queueFamilyIndices =
      util::findQueueFamilies(dvcePtr_->getPhysical(), dvcePtr_->getSurface());

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

  if (vkCreateCommandPool(dvcePtr_->getLogical(), &poolInfo, nullptr,
                          &commandPool_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create graphics command pool!");
  }
}

void Cmdr::createCommandBuffers(int maxFramesInFlight) {
  util::log("Creating command buffers...");
  commandBuffers_.resize(maxFramesInFlight);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = commandPool_;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (uint32_t)commandBuffers_.size();

  if (vkAllocateCommandBuffers(dvcePtr_->getLogical(), &allocInfo,
                               commandBuffers_.data()) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/
const VkCommandPool &Cmdr::getCommandPool() const { return commandPool_; }
const std::vector<VkCommandBuffer> &Cmdr::getCommandBuffers() const {
  return commandBuffers_;
}

/*-----------------------------------------------------------------------------
------------------------------SINGLE-TIMERS------------------------------------
-----------------------------------------------------------------------------*/
VkCommandBuffer Cmdr::beginSingleTimeCommands() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = commandPool_;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(dvcePtr_->getLogical(), &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  return commandBuffer;
}

void Cmdr::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(dvcePtr_->getGraphicsQue(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(dvcePtr_->getGraphicsQue());

  vkFreeCommandBuffers(dvcePtr_->getLogical(), commandPool_, 1, &commandBuffer);
}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Cmdr::cleanup() {
  util::log("Cleaning up cmdr...");

  util::log("Destroying command pool...");
  vkDestroyCommandPool(dvcePtr_->getLogical(), commandPool_, nullptr);
}