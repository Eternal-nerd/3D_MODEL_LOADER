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

/* BASED ON SPECIFIC FONT FILE USED :
total width: 300px
total height: 300px
letter width & height: 30px
rows: 10
col: 10
---- NOT USED LOL
*/
struct LetterQuad {
    std::vector<LetterVertex> vertices = {
        { { -0.1, 0.1 }, { 0.0f, 0.0f } },
        { {  0.1, 0.1 }, { 0.1f, 0.0f } },
        { {  0.1,  -0.1 }, { 0.1f, 0.1f } },
        { { -0.1,  -0.1 }, { 0.0f, 0.1f } },
    };

    std::vector<uint32_t> indices = {
        2,1,0, 3,2,0
    };
};

const float LETTER_OFFSET = 0.1f;

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

    bool visible_ = true;
	
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