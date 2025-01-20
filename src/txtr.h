#pragma once

#include <vulkan/vulkan.h>

#include <string>

#include "cmdr.h"
#include "dvce.h"
#include "util.h"

class Txtr {
public:
  Txtr();
  ~Txtr();

  // to give texture access to stuff
  void setAccessPtrs(const Dvce &dvce, Cmdr &cmdr);

  void create(const std::string &filename);

  const VkImageView &getTextureImageView() const;
  const VkSampler &getTextureSampler() const;

  // used?
  const VkDescriptorImageInfo& getDescriptor() const;

  void cleanup();

private:
  // reference to device:
  const Dvce *dvcePtr_ = nullptr;
  // reference to cmdr:
  Cmdr *cmdrPtr_ = nullptr;

  // name of picture file
  const char *filename_;

  // TEXTURE STUFF
  VkImage textureImage_ = VK_NULL_HANDLE;
  VkDeviceMemory textureImageMemory_ = VK_NULL_HANDLE;
  VkImageView textureImageView_ = VK_NULL_HANDLE;
  VkSampler textureSampler_ = VK_NULL_HANDLE;
  VkImageLayout imageLayout_;
  VkDescriptorImageInfo descriptor_;

  void createTextureImage();
  void createTextureImageView();
  void createTextureSampler();

  // HELPERS
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);

  void updateDescriptor();
};