#include "txtr.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#include <stdexcept>

Txtr::Txtr() {}

Txtr::~Txtr() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Txtr::create(const std::string &filename, const Dvce& dvce, Cmdr& cmdr) {
  util::log("Creating texture...");

  dvcePtr_ = &dvce;
  cmdrPtr_ = &cmdr;

  filename_ = filename.c_str();

  createTextureImage();
  createTextureImageView();
  createTextureSampler();
  updateDescriptor();
}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/
const VkImageView &Txtr::getTextureImageView() const {
  return textureImageView_;
}
const VkSampler &Txtr::getTextureSampler() const { return textureSampler_; }

const VkDescriptorImageInfo& Txtr::getDescriptor() const { return descriptor_; }

/*-----------------------------------------------------------------------------
------------------------------TEXTURE-IMAGE------------------------------------
-----------------------------------------------------------------------------*/
void Txtr::createTextureImage() {
  util::log("Creating texture image...");
  int texWidth, texHeight, texChannels;
  stbi_uc *pixels =
      stbi_load(filename_, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
  VkDeviceSize imageSize = texWidth * texHeight * 4;

  if (!pixels) {
    throw std::runtime_error("failed to load texture image!");
  }

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  util::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory, dvcePtr_->getLogical(),
                     dvcePtr_->getPhysical());

  void *data;
  vkMapMemory(dvcePtr_->getLogical(), stagingBufferMemory, 0, imageSize, 0,
              &data);
  memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(dvcePtr_->getLogical(), stagingBufferMemory);

  stbi_image_free(pixels);

  util::createImage(
      texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage_, textureImageMemory_,
      dvcePtr_->getLogical(), dvcePtr_->getPhysical());

  transitionImageLayout(textureImage_, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_UNDEFINED,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
  copyBufferToImage(stagingBuffer, textureImage_,
                    static_cast<uint32_t>(texWidth),
                    static_cast<uint32_t>(texHeight));
  transitionImageLayout(textureImage_, VK_FORMAT_R8G8B8A8_SRGB,
                        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

  vkDestroyBuffer(dvcePtr_->getLogical(), stagingBuffer, nullptr);
  vkFreeMemory(dvcePtr_->getLogical(), stagingBufferMemory, nullptr);
}

/*-----------------------------------------------------------------------------
------------------------------TEXTURE-IMAGE-VIEW-------------------------------
-----------------------------------------------------------------------------*/
void Txtr::createTextureImageView() {
  util::log("Creating texture image view...");
  textureImageView_ =
      util::createImageView(textureImage_, VK_FORMAT_R8G8B8A8_SRGB,
                            VK_IMAGE_ASPECT_COLOR_BIT, dvcePtr_->getLogical());
}

/*-----------------------------------------------------------------------------
------------------------------TEXTURE-SAMPLER----------------------------------
-----------------------------------------------------------------------------*/
void Txtr::createTextureSampler() {
  util::log("Creating texture sampler...");
  VkPhysicalDeviceProperties properties{};
  vkGetPhysicalDeviceProperties(dvcePtr_->getPhysical(), &properties);

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_FALSE; // FIXME - WTF THIS
  samplerInfo.maxAnisotropy =
      properties.limits.maxSamplerAnisotropy; // FIXME -> 1.0f
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

  if (vkCreateSampler(dvcePtr_->getLogical(), &samplerInfo, nullptr,
                      &textureSampler_) != VK_SUCCESS) {
    throw std::runtime_error("failed to create texture sampler!");
  }
}

/*-----------------------------------------------------------------------------
------------------------------HELPERS------------------------------------------
-----------------------------------------------------------------------------*/
void Txtr::updateDescriptor() {
    descriptor_.sampler = textureSampler_;
    descriptor_.imageView = textureImageView_;
    descriptor_.imageLayout = imageLayout_;
}

void Txtr::transitionImageLayout(VkImage image, VkFormat format,
                                 VkImageLayout oldLayout,
                                 VkImageLayout newLayout) {
  VkCommandBuffer commandBuffer = cmdrPtr_->beginSingleTimeCommands();

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else {
    throw std::invalid_argument("unsupported layout transition!");
  }

  vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0,
                       nullptr, 0, nullptr, 1, &barrier);

  cmdrPtr_->endSingleTimeCommands(commandBuffer);

  imageLayout_ = newLayout;
}

void Txtr::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                             uint32_t height) {
  VkCommandBuffer commandBuffer = cmdrPtr_->beginSingleTimeCommands();

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(commandBuffer, buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  cmdrPtr_->endSingleTimeCommands(commandBuffer);
}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Txtr::cleanup() {
  util::log("Cleaning up texture...");
  // CLEANUP TEXTURES
  util::log("Destroying texture sampler...");
  vkDestroySampler(dvcePtr_->getLogical(), textureSampler_, nullptr);
  util::log("Destroying texture image view...");
  vkDestroyImageView(dvcePtr_->getLogical(), textureImageView_, nullptr);
  util::log("Destroying texture image...");
  vkDestroyImage(dvcePtr_->getLogical(), textureImage_, nullptr);
  util::log("Destroying texture image memory...");
  vkFreeMemory(dvcePtr_->getLogical(), textureImageMemory_, nullptr);
}