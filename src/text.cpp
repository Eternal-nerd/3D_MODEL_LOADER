#include "text.h"

Text::Text() {}
Text::~Text() {}


void Text::draw(VkCommandBuffer commandBuffer) {
    for (auto renderable : letterQuads_) {
        renderable.bind(commandBuffer);
        renderable.draw(commandBuffer);
    }
}