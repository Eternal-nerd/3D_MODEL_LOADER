#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "types.h"
#include "dvce.h"
#include "cmdr.h"
#include "util.h"

struct RenderableData {
	std::vector<Vertex> vertices = {};
	std::vector<uint32_t> indices = {};
};

struct RenderableAccess {
	const Dvce* dvcePtr = nullptr;
	Cmdr* cmdrPtr = nullptr;
};

class Renderable {
public:

  Renderable();
  ~Renderable();
  
  void init(int num, const RenderableData& data, const RenderableAccess& access);

  void bind(VkCommandBuffer commandBuffer);

  void draw(VkCommandBuffer commandBuffer);

  void cleanup();

  glm::vec3 position_ = { 0, 0, 0 };

private:

	int num_ = -1;

	RenderableData data_;
	RenderableAccess access_;

	VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkBuffer indexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;
  
	void createVertexBuffer();
	void createIndexBuffer();


};
