#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>

// DEBUGGING

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

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

// UTILITIES
std::vector<char> readFile(const std::string &filename);

// BUFFERS / MEM Manage

/*
class BufferManager {
public:
    BufferManager(VkDevice& device, VkPhysicalDevice& physicalDevice, );
    ~BufferManager();

private:


};



void createBuffer2(VkDevice& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
void copyBuffer2(VkDevice& device, VkCommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
uint32_t findMemoryType2(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);

// command buffer helpers
VkCommandBuffer beginSingleTimeCommands2(VkDevice& device, VkCommandPool& commandPool);
void endSingleTimeCommands2(VkDevice& device, VkCommandBuffer commandBuffer, VkCommandPool& commandPool, VkQueue& graphicsQueue);

*/