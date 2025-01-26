#include "renderable.h"

Renderable::Renderable() {}
Renderable::~Renderable() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Renderable::init(int num, const RenderableData& data, const RenderableAccess& access) {
  util::log("Initializing Renderable...");

  num_ = num;
  data_ = data;
  access_ = access;

  createVertexBuffer();
  createIndexBuffer();
}

/*-----------------------------------------------------------------------------
------------------------------BINDING------------------------------------------
-----------------------------------------------------------------------------*/

void Renderable::bind(VkCommandBuffer commandBuffer) {
    VkBuffer vertexBuffers[] = { vertexBuffer_ };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
}


/*-----------------------------------------------------------------------------
------------------------------CHANGE-TEXTURE-----------------------------------
-----------------------------------------------------------------------------*/
void Renderable::setTextureIndex(int texIndex) {
    // recreate buffers (destroy them)
    cleanup();

    for (int i = 0; i < data_.vertices.size(); i++) {
        // no safeguard LOL
        data_.vertices[i].texIndex = texIndex;
    }
    std::cout << "test?\n";
    createVertexBuffer();
    createIndexBuffer();
}

/*-----------------------------------------------------------------------------
------------------------------DRAWING------------------------------------------
-----------------------------------------------------------------------------*/

void Renderable::draw(VkCommandBuffer commandBuffer) {
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(data_.indices.size()), 1, 0, 0, num_);
}

/*-----------------------------------------------------------------------------
------------------------------VERTEX-BUFFER------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::createVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(data_.vertices[0]) * data_.vertices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  util::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory, access_.dvcePtr->getLogical(),
                    access_.dvcePtr->getPhysical());

  void *data;
  vkMapMemory(access_.dvcePtr->getLogical(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, data_.vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(access_.dvcePtr->getLogical(), stagingBufferMemory);

  util::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer_, vertexBufferMemory_,
      access_.dvcePtr->getLogical(), access_.dvcePtr->getPhysical());

  util::copyBuffer(stagingBuffer, vertexBuffer_, bufferSize, *access_.cmdrPtr);

  vkDestroyBuffer(access_.dvcePtr->getLogical(), stagingBuffer, nullptr);
  vkFreeMemory(access_.dvcePtr->getLogical(), stagingBufferMemory, nullptr);
}


/*-----------------------------------------------------------------------------
------------------------------INDEX-BUFFER-------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::createIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(data_.indices[0]) * data_.indices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  util::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory, access_.dvcePtr->getLogical(),
                        access_.dvcePtr->getPhysical());

  void *data;
  vkMapMemory(access_.dvcePtr->getLogical(), stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, data_.indices.data(), (size_t)bufferSize);
  vkUnmapMemory(access_.dvcePtr->getLogical(), stagingBufferMemory);

  util::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer_, indexBufferMemory_,
      access_.dvcePtr->getLogical(), access_.dvcePtr->getPhysical());

  util::copyBuffer(stagingBuffer, indexBuffer_, bufferSize, *access_.cmdrPtr);

  vkDestroyBuffer(access_.dvcePtr->getLogical(), stagingBuffer, nullptr);
  vkFreeMemory(access_.dvcePtr->getLogical(), stagingBufferMemory, nullptr);
}

/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Renderable::cleanup() {
  util::log("Cleaning up Renderable...");

  vkDestroyBuffer(access_.dvcePtr->getLogical(), indexBuffer_, nullptr);
  vkFreeMemory(access_.dvcePtr->getLogical(), indexBufferMemory_, nullptr);
  vkDestroyBuffer(access_.dvcePtr->getLogical(), vertexBuffer_, nullptr);
  vkFreeMemory(access_.dvcePtr->getLogical(), vertexBufferMemory_, nullptr);


}



