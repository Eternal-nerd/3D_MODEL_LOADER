#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "types.h"
#include "util.h"

struct RenderableData {
  const std::vector<Vertex> vertices;
  const std::vector<uint16_t> indices;  
};

class Renderable {
public:
  Renderable();
  ~Renderable();
  
  void init();

  void bind();

  void draw();

  void cleanup();

private:
  VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
  VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
  VkBuffer indexBuffer_ = VK_NULL_HANDLE;
  VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;
  
  void createVertexBuffer();
  void createIndexBuffer();


};
