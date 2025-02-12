#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <iostream>

#include "types.h"
#include "dvce.h"
#include "cmdr.h"
#include "util.h"

class Renderable {
public:

  Renderable();
  ~Renderable();

  void init(int id, const RenderableData& data, const GfxAccess& access);

  void draw(VkCommandBuffer commandBuffer);

  void setTextureIndex(int texIndex);

  int getId();

  void cleanup();

  glm::vec3 position_ = { 0, 0, 0 };

private:
	int id_ = -1;

	RenderableData data_;
	GfxAccess access_;

	VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
	VkBuffer indexBuffer_ = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory_ = VK_NULL_HANDLE;
  
	void createVertexBuffer();
	void createIndexBuffer();


};
