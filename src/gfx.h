#pragma once

#include <vulkan/vulkan.h>

#include "dvce.h"
#include "swpchn.h"
#include "util.h"
#include "txtr.h"
#include "cmdr.h"
#include "synchro.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

// FIXME REMOVE:
// INPUT DATA
const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f, 1.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, -0.5f, 1.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f, 1.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

	{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
	{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}} };

const std::vector<uint16_t> input_indices = {
	0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 4, 1, 2, 6, 3, 0 // FIXME
};
// END REMOVE

class Gfx {
public:
	Gfx();
	~Gfx();

	void init();

	void drawRenderable();

	// FIXME REMOVE
	void tempDrawFrame();

	void deviceWaitIdle();

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
	VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;

	Cmdr cmdr_;

	Txtr txtr_;

	std::vector<VkDescriptorSet> descriptorSets_;

	Synchro synchro_;

	uint32_t currentFrame_ = 0;

	void createRenderPass();
	void createDescriptorSetLayout();
	void createGraphicsPipeline();

	// Maybe not here?
	void createDescriptorPool();

	// TODO REMOVE AND REPLACE WITH RENDERABLE CLASS STUFF
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();
	VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkBuffer indexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;
	std::vector<VkBuffer> uniformBuffers_;
	std::vector<VkDeviceMemory> uniformBuffersMemory_;
	std::vector<void*> uniformBuffersMapped_;
	// END REMOVE

	// this can stay here
	void createDescriptorSets();

	// TODO MOVE TO CMDR CLASS? 
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
	std::vector<VkCommandBuffer> commandBuffers_;

	// TODO FIND LOCATION FOR:
	void updateUniformBuffer(uint32_t currentImage);

};