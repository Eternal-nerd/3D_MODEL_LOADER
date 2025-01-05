#pragma once

#include <vulkan/vulkan.h>

#include "dvce.h"
#include "swpchn.h"
#include "util.h"
#include "txtr.h"
#include "cmdr.h"

class Gfx {
public:
	Gfx();
	~Gfx();

	void init();

	void drawRenderable();

	void cleanup();

	void setWindowPtr(SDL_Window* window);


private:
	// SDL WINDOW
	SDL_Window* window_ = nullptr;

	Dvce dvce_;
	Swpchn swpchn_;

	VkRenderPass renderPass_ = VK_NULL_HANDLE;

	VkDescriptorSetLayout descriptorSetLayout_ = VK_NULL_HANDLE;
	VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
	VkPipeline graphicsPipeline_ = VK_NULL_HANDLE;

	Cmdr cmdr_;

	Txtr txtr_;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();

};