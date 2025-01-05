#include "swpchn.h"

#include <stdexcept>

Swpchn::Swpchn() {}

Swpchn::~Swpchn() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::setDvcePtr(const Dvce& dvce) {
	dvcePtr_ = &dvce;
}

void Swpchn::preInit() {
	util::log("Getting some swapchain details...");
	swapChainSupport_ = util::querySwapChainSupport(dvcePtr_->getPhysical(), dvcePtr_->getSurface());
	presentMode_ = util::chooseSwapPresentMode(swapChainSupport_.presentModes);
	surfaceFormat_ = util::chooseSwapSurfaceFormat(swapChainSupport_.formats);
	swapChainExtent_ = util::chooseSwapExtent(swapChainSupport_.capabilities, dvcePtr_->getWindowPtr());
	swapChainImageFormat_ = surfaceFormat_.format;
}

void Swpchn::setRenderPassPtr(const VkRenderPass& renderPass) {
	renderPassPtr_ = &renderPass;
}


void Swpchn::init() {
	util::log("Initializing swpchn...");

	createSwapChain();
	createImageViews();
	createDepthResources();
	createFramebuffers();

}

/*-----------------------------------------------------------------------------
-----------------------------GETTERS-------------------------------------------
-----------------------------------------------------------------------------*/
const VkFormat& Swpchn::getSwapFormat() const {	return swapChainImageFormat_; }
const VkExtent2D& Swpchn::getSwapExtent() const { return swapChainExtent_; }


/*-----------------------------------------------------------------------------
-----------------------------SWAPCHAIN-----------------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::createSwapChain() {
	util::log("Creating swapchain...");

	uint32_t imageCount = swapChainSupport_.capabilities.minImageCount + 1;
	if (swapChainSupport_.capabilities.maxImageCount > 0 &&
		imageCount > swapChainSupport_.capabilities.maxImageCount) {
		imageCount = swapChainSupport_.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = dvcePtr_->getSurface();

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat_.format;
	createInfo.imageColorSpace = surfaceFormat_.colorSpace;
	createInfo.imageExtent = swapChainExtent_;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = util::findQueueFamilies(dvcePtr_->getPhysical(), dvcePtr_->getSurface());
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(),
									 indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport_.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode_;
	createInfo.clipped = VK_TRUE;

	if (vkCreateSwapchainKHR(dvcePtr_->getLogical(), &createInfo, nullptr, &swapChain_) !=
		VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(dvcePtr_->getLogical(), swapChain_, &imageCount, nullptr);
	swapChainImages_.resize(imageCount);
	vkGetSwapchainImagesKHR(dvcePtr_->getLogical(), swapChain_, &imageCount,
		swapChainImages_.data());
}

void Swpchn::recreateSwapChain() {
	int width = 0, height = 0;
	SDL_GetWindowSizeInPixels(dvcePtr_->getWindowPtr(), &width, &height);

	while (width == 0 || height == 0) {
		SDL_GetWindowSizeInPixels(dvcePtr_->getWindowPtr(), &width, &height);
		// FIXME SDL_PollEvent(&event_);
	}

	dvcePtr_->waitIdle();

	cleanup();

	createSwapChain();
	createImageViews();
	createDepthResources();
	createFramebuffers();
}

/*-----------------------------------------------------------------------------
------------------------------IMAGE-VIEWS--------------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::createImageViews() {
	util::log("Creating image views...");
	swapChainImageViews_.resize(swapChainImages_.size());

	for (uint32_t i = 0; i < swapChainImages_.size(); i++) {
		swapChainImageViews_[i] = util::createImageView(swapChainImages_[i], swapChainImageFormat_, VK_IMAGE_ASPECT_COLOR_BIT, dvcePtr_->getLogical());
	}
}


/*-----------------------------------------------------------------------------
-----------------------------DEPTH-RESOURCES-----------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::createDepthResources() {
	util::log("Creating depth resources...");
	VkFormat depthFormat = util::findDepthFormat(dvcePtr_->getPhysical());

	util::createImage(
		swapChainExtent_.width, swapChainExtent_.height, depthFormat,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage_, depthImageMemory_, dvcePtr_->getLogical(), dvcePtr_->getPhysical());
	depthImageView_ =
		util::createImageView(depthImage_, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, dvcePtr_->getLogical());
}


/*-----------------------------------------------------------------------------
------------------------------FRAMEBUFFERS-------------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::createFramebuffers() {
	util::log("Creating framebuffers...");
	swapChainFramebuffers_.resize(swapChainImageViews_.size());

	for (size_t i = 0; i < swapChainImageViews_.size(); i++) {
		std::array<VkImageView, 2> attachments = { swapChainImageViews_[i],
												  depthImageView_ };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = *renderPassPtr_;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChainExtent_.width;
		framebufferInfo.height = swapChainExtent_.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(dvcePtr_->getLogical(), &framebufferInfo, nullptr,
			&swapChainFramebuffers_[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Swpchn::cleanup() {
	util::log("Cleaning up swpchn...");

	util::log("Destroying swapchain depth resources...");
	vkDestroyImageView(dvcePtr_->getLogical(), depthImageView_, nullptr);
	vkDestroyImage(dvcePtr_->getLogical(), depthImage_, nullptr);
	vkFreeMemory(dvcePtr_->getLogical(), depthImageMemory_, nullptr);

	util::log("Destroying swapchain frame buffers...");
	for (auto framebuffer : swapChainFramebuffers_) {
		vkDestroyFramebuffer(dvcePtr_->getLogical(), framebuffer, nullptr);
	}

	util::log("Destroying swapchain image views...");
	for (auto imageView : swapChainImageViews_) {
		vkDestroyImageView(dvcePtr_->getLogical(), imageView, nullptr);
	}

	util::log("Destroying swapchain...");
	vkDestroySwapchainKHR(dvcePtr_->getLogical(), swapChain_, nullptr);

}
