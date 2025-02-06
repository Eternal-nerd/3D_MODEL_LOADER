#pragma once

#include <vulkan/vulkan.h>

#include <vector>
// for letter dictionary
#include <unordered_map>

#include "renderable.h"
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
    std::vector<Vertex> vertices = {
        { { -0.1, 0.1,  0.0 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { {  0.1, 0.1,  0.0 }, {1.0, 1.0, 1.0}, { 0.1f, 0.0f }, 0 },
        { {  0.1,  -0.1,  0.0 }, {1.0, 1.0, 1.0}, { 0.1f, 0.1f }, 0 },
        { { -0.1,  -0.1,  0.0 }, {1.0, 1.0, 1.0}, { 0.0f, 0.1f }, 0 },
    };

    std::vector<uint32_t> indices = {
        2,1,0, 3,2,0
    };
};

class Text {
public:
	Text();
	~Text();

    void draw(VkCommandBuffer commandBuffer);


    bool visible_ = true;

private:
	std::vector<Renderable> letterQuads_ = {};



};