#include "renderable.h"

Renderable::Renderable() {}
Renderable::~Renderable() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Renderable::init() {
  util::log("Initializing Renderable...");

}

/*-----------------------------------------------------------------------------
------------------------------BINDING------------------------------------------
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
------------------------------DRAWING------------------------------------------
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
------------------------------VERTEX-BUFFER------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::createVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  util::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory, dvce_.getLogical(),
                     dvce_.getPhysical());

  void *data;
  vkMapMemory(dvce_.getLogical(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(dvce_.getLogical(), stagingBufferMemory);

  util::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer_, vertexBufferMemory_,
      dvce_.getLogical(), dvce_.getPhysical());

  util::copyBuffer(stagingBuffer, vertexBuffer_, bufferSize, cmdr_);

  vkDestroyBuffer(dvce_.getLogical(), stagingBuffer, nullptr);
  vkFreeMemory(dvce_.getLogical(), stagingBufferMemory, nullptr);
}


/*-----------------------------------------------------------------------------
------------------------------INDEX-BUFFER-------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::createIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(input_indices[0]) * input_indices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  util::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory, dvce_.getLogical(),
                     dvce_.getPhysical());

  void *data;
  vkMapMemory(dvce_.getLogical(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, input_indices.data(), (size_t)bufferSize);
  vkUnmapMemory(dvce_.getLogical(), stagingBufferMemory);

  util::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer_, indexBufferMemory_,
      dvce_.getLogical(), dvce_.getPhysical());

  util::copyBuffer(stagingBuffer, indexBuffer_, bufferSize, cmdr_);

  vkDestroyBuffer(dvce_.getLogical(), stagingBuffer, nullptr);
  vkFreeMemory(dvce_.getLogical(), stagingBufferMemory, nullptr);
}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::cleanup() {
  util::log("Cleaning up Renderable...");
  
  // TODO
  /*vkDestroyBuffer(dvce_.getLogical(), indexBuffer_, nullptr);
  vkFreeMemory(dvce_.getLogical(), indexBufferMemory_, nullptr);
  vkDestroyBuffer(dvce_.getLogical(), vertexBuffer_, nullptr);
  vkFreeMemory(dvce_.getLogical(), vertexBufferMemory_, nullptr);*/

}



