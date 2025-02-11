#pragma once

#include <vulkan/vulkan.h>

#include <vector>
#include <iostream>


#include "gfx.h"
#include "txtr.h"
#include "renderable.h"
#include "util.h"
#include "types.h"

// Max. number of chars the text overlay buffer can hold
#define TEXTOVERLAY_MAX_CHAR_COUNT 2048

const float LETTER_OFFSET_X = 0.06f;
const float LETTER_OFFSET_Y = 0.1f;

// mostly self-contained text overlay class based on Sascha Willems implementation
class Text {
public:
	Text();
	~Text();

	// init text overlay
	void init(const GfxAccess& access);

	// USED BY ENGINE:
	void beginTextUpdate();
	void addText(const std::string& text, float xPos, float yPos);
	void endTextUpdate();

	// draw each char
    void draw(VkCommandBuffer commandBuffer);

	// delete resources
	void cleanup();

    bool visible_ = false;
	
	float scale = 1.f;

private:
	// reference to gfx class:
	GfxAccess access_;

    Txtr fontTexture_;

	// NOT using renderable cause we want just one simple big buffer to index into..
	VkBuffer buffer_;
	VkDeviceMemory memory_;

	VkDescriptorPool descriptorPool_;
	VkDescriptorSetLayout descriptorSetLayout_;
	VkDescriptorSet descriptorSet_;
	VkPipelineLayout pipelineLayout_;
	VkPipelineCache pipelineCache_;
	VkPipeline pipeline_;

	uint32_t numLetters_ = 0;

	// Pointer to mapped vertex buffer
	glm::vec4* mapped = nullptr;

	void prepareResources();
	void preparePipeline();

	// letter quad size helpers
	// TODO?

	// Helpers for texture indexing
	float getXOffset(char c);
	float getYOffset(char c);

};