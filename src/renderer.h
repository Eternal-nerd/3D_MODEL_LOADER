#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>

#include "data.h"
#include "types.h"
#include "util.h"

class Renderer {
  /*****************************PUBLIC*********************************************/
public:
  /*--------------------------------------------------------------------------------
  ------------------------------METHODS---------------------------------------------
  --------------------------------------------------------------------------------*/

  Renderer();
  ~Renderer();

  void init();

  void drawFrame();

  void cleanup();

  // FIXME
  void setWindowPtr(SDL_Window *window);
  void deviceWaitIdle();

  /*--------------------------------------------------------------------------------
  ------------------------------VARIABLES-------------------------------------------
  --------------------------------------------------------------------------------*/

  // NAAAAA

  /*****************************PRIVATE*********************************************/
private:
  /*--------------------------------------------------------------------------------
  ------------------------------VARIABLES-------------------------------------------
  --------------------------------------------------------------------------------*/
  // SDL WINDOW
  SDL_Window *window_ = nullptr;

  // FIXME make private asap
  VkInstance instance_ = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT debugMessenger_ = VK_NULL_HANDLE;

  VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
  VkDevice device_ = VK_NULL_HANDLE;

  VkQueue graphicsQueue_ = VK_NULL_HANDLE;
  VkQueue presentQueue_ = VK_NULL_HANDLE;

  VkSurfaceKHR surface_ = VK_NULL_HANDLE;

  VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;
  std::vector<VkImage> swapChainImages_;
  VkFormat swapChainImageFormat_;
  VkExtent2D swapChainExtent_;

  std::vector<VkImageView> swapChainImageViews_;
  std::vector<VkFramebuffer> swapChainFramebuffers_;

  VkRenderPass renderPass_ = VK_NULL_HANDLE;
  VkDescriptorSetLayout descriptorSetLayout_ = VK_NULL_HANDLE;
  VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
  VkPipeline graphicsPipeline_ = VK_NULL_HANDLE;

  VkCommandPool commandPool_ = VK_NULL_HANDLE;

  VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
  VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
  VkBuffer indexBuffer_ = VK_NULL_HANDLE;
  VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;

  std::vector<VkBuffer> uniformBuffers_;
  std::vector<VkDeviceMemory> uniformBuffersMemory_;
  std::vector<void *> uniformBuffersMapped_;

  VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;
  std::vector<VkDescriptorSet> descriptorSets_;

  std::vector<VkCommandBuffer> commandBuffers_;

  std::vector<VkSemaphore> imageAvailableSemaphores_;
  std::vector<VkSemaphore> renderFinishedSemaphores_;
  std::vector<VkFence> inFlightFences_;
  uint32_t currentFrame_ = 0;

  // TEXTURE STUFF
  VkImage textureImage_ = VK_NULL_HANDLE;
  VkDeviceMemory textureImageMemory_ = VK_NULL_HANDLE;
  VkImageView textureImageView_ = VK_NULL_HANDLE;
  VkSampler textureSampler_ = VK_NULL_HANDLE;

  // DEPTH
  VkImage depthImage_ = VK_NULL_HANDLE;
  VkDeviceMemory depthImageMemory_ = VK_NULL_HANDLE;
  VkImageView depthImageView_ = VK_NULL_HANDLE;

  /*--------------------------------------------------------------------------------
  ------------------------------METHODS---------------------------------------------
  --------------------------------------------------------------------------------*/

  // VK INSTANCE
  void createInstance();

  // SDL/VK SURFACE
  void createSurface();
  std::vector<const char *> getRequiredExtensions();

  // VK DEBUG/VALIDATION
  void setupDebugMessenger();
  bool checkValidationLayerSupport();

  // VK PHYSCIAL DEVICE
  void pickPhysicalDevice();
  bool isDeviceSuitable(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

  // VK LOGICAL DEVICE
  void createLogicalDevice();

  // SWAPCHAIN
  void createSwapChain();
  void cleanupSwapChain();
  void recreateSwapChain();

  VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<VkSurfaceFormatKHR> &availableFormats);
  VkPresentModeKHR chooseSwapPresentMode(
      const std::vector<VkPresentModeKHR> &availablePresentModes);
  VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

  // DEPTH
  void createDepthResources();
  VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat findDepthFormat();

  // FRAMEBUFFER?
  void createFramebuffers();

  // IMAGE VIEWS:
  VkImageView createImageView(VkImage image, VkFormat format,
                              VkImageAspectFlags aspectFlags);
  void createImageViews();

  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();

  // TEXTURES
  void createTextureImage();
  void createImage(uint32_t width, uint32_t height, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout);
  void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width,
                         uint32_t height);
  void createTextureImageView();
  void createTextureSampler();

  // TODO EXPLAIN THIS

  void createCommandPool();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void createDescriptorPool();
  void createDescriptorSets();

  void createCommandBuffers();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  void createSyncObjects();
  void updateUniformBuffer(uint32_t currentImage);

  VkShaderModule createShaderModule(const std::vector<char> &code);
};
