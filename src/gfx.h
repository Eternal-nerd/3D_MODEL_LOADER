#pragma once

#include <vulkan/vulkan.h>

#include "cmdr.h"
#include "dvce.h"
#include "swpchn.h"
#include "synchro.h"
#include "txtr.h"
#include "util.h"
#include "renderable.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

class Gfx {
public:
  Gfx();
  ~Gfx();

  void init();

  VkCommandBuffer beginFrame();
  
  // THIS WHERE DRAWS HAPPEN in between the begin and end

  void endFrame(VkCommandBuffer commandBuffer);

  void deviceWaitIdle();

  // getter for renderable access
  void getRenderableAccess(RenderableAccess& access);

  void cleanupStart();
  void cleanupEnd();

  void setWindowPtr(SDL_Window *window);

private:
  // SDL WINDOW
  SDL_Window *window_ = nullptr;

  Dvce dvce_;
  Swpchn swpchn_;

  VkRenderPass renderPass_ = VK_NULL_HANDLE;

  VkDescriptorSetLayout descriptorSetLayout_ = VK_NULL_HANDLE;
  VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
  VkPipeline graphicsPipeline_ = VK_NULL_HANDLE;
  VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;

  Cmdr cmdr_;

  Txtr txtr_;

  std::vector<VkBuffer> uniformBuffers_;
  std::vector<VkDeviceMemory> uniformBuffersMemory_;
  std::vector<void *> uniformBuffersMapped_;

  std::vector<VkDescriptorSet> descriptorSets_;

  Synchro synchro_;

  uint32_t currentFrame_ = 0;

  uint32_t imageIndex_ = 0;

  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();

  void createUniformBuffers();
  void updateUniformBuffer(uint32_t currentImage);

  // Maybe not here?
  void createDescriptorPool();
  void createDescriptorSets();

  // TODO MOVE TO CMDR CLASS?
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
};
