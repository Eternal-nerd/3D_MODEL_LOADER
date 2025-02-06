#pragma once

#include <vector>
#include <vulkan/vulkan.h>

#include "dvce.h"
#include "util.h"

class Swpchn {
public:
  Swpchn();
  ~Swpchn();

  void init(const Dvce& dvce, const VkRenderPass& renderPass);

  const VkFormat &getSwapFormat() const;
  const VkExtent2D &getSwapExtent() const;
  const VkSwapchainKHR &getSwapChain() const;
  const std::vector<VkFramebuffer> &getFrameBuffers() const;

  void recreate();

  void cleanup();

private:
  // reference to device:
  const Dvce *dvcePtr_ = nullptr;

  // reference to renderpass
  const VkRenderPass *renderPassPtr_ = nullptr;

  VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;
  std::vector<VkImage> swapChainImages_;

  // details
  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;

  std::vector<VkImageView> swapChainImageViews_;
  std::vector<VkFramebuffer> swapChainFramebuffers_;

  // DEPTH
  VkImage depthImage_ = VK_NULL_HANDLE;
  VkDeviceMemory depthImageMemory_ = VK_NULL_HANDLE;
  VkImageView depthImageView_ = VK_NULL_HANDLE;

  void createSwapChain();
  void recreateSwapChain();

  void createImageViews();
  void createDepthResources();
  void createFramebuffers();
};