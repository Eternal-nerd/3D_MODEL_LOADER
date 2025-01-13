#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include "types.h"
#include "dvce.h"
#include "cmdr.h"
#include "util.h"

struct RenderableData {
	std::vector<Vertex> vertices = {};
	std::vector<uint16_t> indices = {};
};

struct RenderableAccess {
	const Dvce* dvcePtr = nullptr;
	Cmdr* cmdrPtr = nullptr;
};

class Renderable {
public:

  Renderable();
  ~Renderable();
  
  void init(const std::string& name, const RenderableData& data, const RenderableAccess& access);

  void bind(VkCommandBuffer commandBuffer);

  void draw(VkCommandBuffer commandBuffer);

  void cleanup();

private:
	RenderableData data_;
	RenderableAccess access_;

	std::string name_;

	// TODO IMPLEMENT
	struct Vertices {
		int count;
		VkBuffer buffer;
		VkDeviceMemory memory;
	} vertices_;

	struct Indices {
		int count;
		VkBuffer buffer;
		VkDeviceMemory memory;
	} indices_;

	//glm::vec3 position_; // TODO

	VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkBuffer indexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;
  
	void createVertexBuffer();
	void createIndexBuffer();


};
