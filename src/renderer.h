#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <iostream>

#include "util.h"
#include "data.h"
#include "types.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	void init();

	void drawFrame();
	
	void cleanup();

	//FIXME
	void setWindowPtr(SDL_Window* window);
	void deviceWaitIdle();


	//FIXME!!!!!
	void createSwapChain();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

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

private:
	SDL_Window* window_ = nullptr;

	void createInstance();
	void setupDebugMessenger();

	void pickPhysicalDevice();
	void createLogicalDevice();

	void createSurface();

	//void createSwapChain();
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(
		const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

	// Helpers
	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	bool isDeviceSuitable(VkPhysicalDevice device);
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	//QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
};
