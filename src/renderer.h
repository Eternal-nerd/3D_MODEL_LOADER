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

    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;

    VkCommandPool commandPool = VK_NULL_HANDLE;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
    std::vector<VkDescriptorSet> descriptorSets;

    std::vector<VkCommandBuffer> commandBuffers;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame = 0;

    // TEXTURE STUFF
    VkImage textureImage = VK_NULL_HANDLE;
    VkDeviceMemory textureImageMemory = VK_NULL_HANDLE;
    VkImageView textureImageView = VK_NULL_HANDLE;
    VkSampler textureSampler = VK_NULL_HANDLE;

    // DEPTH
    VkImage depthImage = VK_NULL_HANDLE;
    VkDeviceMemory depthImageMemory = VK_NULL_HANDLE;
    VkImageView depthImageView = VK_NULL_HANDLE;

/*--------------------------------------------------------------------------------
------------------------------METHODS---------------------------------------------
--------------------------------------------------------------------------------*/

  // VK INSTANCE
  void createInstance();

  // SDL/VK SURFACE
  void createSurface();
  std::vector<const char*> getRequiredExtensions();
  
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
  VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
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
      VkMemoryPropertyFlags properties, VkImage& image,
      VkDeviceMemory& imageMemory);
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
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties, VkBuffer& buffer,
      VkDeviceMemory& bufferMemory);

  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  uint32_t findMemoryType(uint32_t typeFilter,
      VkMemoryPropertyFlags properties);

  void createCommandBuffers();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

  void createSyncObjects();
  void updateUniformBuffer(uint32_t currentImage);

  VkShaderModule createShaderModule(const std::vector<char>& code);
};
