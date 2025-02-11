#include "renderable.h"

Renderable::Renderable() {}
Renderable::~Renderable() {}

/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Renderable::init(int id, const RenderableData& data, const GfxAccess& access) {
  util::log("Initializing Renderable...");

  id_ = id;
  data_ = data;
  access_ = access;

  createVertexBuffer();
  createIndexBuffer();
}

/*-----------------------------------------------------------------------------
------------------------------GETTER-------------------------------------------
-----------------------------------------------------------------------------*/
int Renderable::getId() {
    return id_;
}

/*
// FIXME NOT READY TO USE!
void Renderable::initGLTF(int num, const RenderableAccess& access, const std::string& filename) {
    util::log("Initializing GLTF Renderable...");
    isGLTF_ = true;

    num_ = num;
    access_ = access;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err;
    std::string warn;

    //bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename); // for binary glTF(.glb)
    gltfModel_ = &model;

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }
    
    if (!ret) {
        throw std::runtime_error("failed to load GLTF model from file!  ");
    }



    // MUST CREATE BUFFERS / TEXTURE DESCRIPTORS???s HERE
}*/


/*-----------------------------------------------------------------------------
------------------------------BINDING------------------------------------------
-----------------------------------------------------------------------------*/

void Renderable::bind(VkCommandBuffer commandBuffer) {
    VkDeviceSize offsets =  0;
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer_, &offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
}


/*-----------------------------------------------------------------------------
------------------------------DRAWING------------------------------------------
-----------------------------------------------------------------------------*/

void Renderable::draw(VkCommandBuffer commandBuffer) {
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(data_.indices.size()), 1, 0, 0, id_);
}

/*-----------------------------------------------------------------------------
------------------------------CHANGE-TEXTURE-----------------------------------
-----------------------------------------------------------------------------*/
void Renderable::setTextureIndex(int texIndex) {
    // recreate buffers (destroy them)
    cleanup();

    for (int i = 0; i < data_.vertices.size(); i++) {
        data_.vertices[i].texIndex = texIndex;
    }
    createVertexBuffer();
    createIndexBuffer();
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



