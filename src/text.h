#pragma once

#include <vulkan/vulkan.h>

#include <vector>
// for letter dictionary
#include <unordered_map>

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


// mostly self-contained text overlay class based on Sascha Willems implementation
class Text {
public:
	Text();
	~Text();

	// init text overlay
	void init(const GfxAccess& access);

	// draw each char
    void draw(VkCommandBuffer commandBuffer);

	// delete resources
	void cleanup();

    bool visible_ = true;
	enum TextAlign { alignLeft, alignCenter, alignRight };
	uint32_t numLetters;

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

	// Passed from the gfx access
	VkRenderPass renderPass_;
	
	// FIXME - can just get this from device?
	VkQueue queue_;
	
	// FIXME - just get this from swapchain pointer
	uint32_t* frameBufferWidth;
	uint32_t* frameBufferHeight;
	
	float scale_;

	// Pointer to mapped vertex buffer
	glm::vec4* mapped = nullptr;


	void prepareResources();
	void preparePipeline();


};