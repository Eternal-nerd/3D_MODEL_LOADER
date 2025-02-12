#pragma once

// LATER
//#include "../libs/tiny_gltf.h"

#include "../libs/tiny_obj_loader.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

#include "cmdr.h"
#include "types.h"

class Cmdr;

namespace util {

    float roundF(float f, int decimalPlaces);

    // noob logging function
    // TODO USE TEMPLATES TO ALLOW MULTIPLE ARGUMENTS
    void log(const std::string &s);

    void printMat4(const glm::mat4& mat);

    void printVec3(const glm::vec3& vec);

    void printMouseMove(const SDL_MouseMotionEvent& move);

    // GENERAL UTILITIES
    std::vector<char> readFile(const std::string &filename);

    // Vertex/index processing
    RenderableData getObjData(const std::string obj_filename);

    // SHADER
    VkShaderModule createShaderModule(const std::vector<char> &code,
                                      const VkDevice &device);

    // DEBUGGING
    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                       VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
    void populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    // SDL DETAILS
    std::vector<const char *> getRequiredExtensions();

    // VULKAN DETAILS
    bool checkValidationLayerSupport(
        const std::vector<const char *> &validationLayers);
    bool checkDeviceExtensionSupport(
        VkPhysicalDevice physicalDevice,
        const std::vector<const char *> &deviceExtensions);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice physicalDevice,
                                         VkSurfaceKHR surface);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties,
                            const VkPhysicalDevice &physicalDevice);

    // SWAPCHAIN details
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice physicalDevice,
                                                  VkSurfaceKHR surface);
    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR> &availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR> &availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                                SDL_Window *window);

    // Depth
    VkFormat findDepthFormat(const VkPhysicalDevice &physicalDevice);
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates,
                                 VkImageTiling tiling,
                                 VkFormatFeatureFlags features,
                                 const VkPhysicalDevice &physicalDevice);

    // Image shit
    VkImageView createImageView(VkImage image, VkFormat format,
                                VkImageAspectFlags aspectFlags,
                                const VkDevice &device);
    void createImage(uint32_t width, uint32_t height, VkFormat format,
                     VkImageTiling tiling, VkImageUsageFlags usage,
                     VkMemoryPropertyFlags properties, VkImage &image,
                     VkDeviceMemory &imageMemory, const VkDevice &device,
                     const VkPhysicalDevice &physicalDevice);

    // BUFFERS
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                      VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory, const VkDevice &device,
                      const VkPhysicalDevice &physicalDevice);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size,
                    Cmdr &cmdr);
} // namespace util