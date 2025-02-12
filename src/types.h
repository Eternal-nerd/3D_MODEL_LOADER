#pragma once

#include <SDL3/SDL.h>
#include <vulkan/vulkan.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <array>
#include <optional>
#include <vector>

#include <assert.h>

const int MAX_MODELS = 512;

class Dvce;
class Cmdr;
class Swpchn;

struct GfxAccess {
    // get queues throught device
    const Dvce* dvcePtr = nullptr;
    Cmdr* cmdrPtr = nullptr;
    // for text overlay:
    const Swpchn* swpchnPtr = nullptr;
    VkRenderPass vkRenderpass = VK_NULL_HANDLE;

};

struct UniformBufferObject {
    alignas(16) glm::mat4 model[MAX_MODELS];
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

// FIXME
struct color {
    int r;
    int g;
    int b;
};

struct CamPhysicsAttributes {
    glm::vec3 velocity = { 0,0,0 };
    glm::vec3 position = { 0,0,0 };
    bool grounded = true;
    bool jumping = false;
};

struct KeyStates {
    bool q = false;
    bool w = false;
    bool e = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool space = false;
    bool ctrl = false;
    bool shift = false;
    bool n = false; // NOCLIP
    bool f3 = false; // debug text
    bool p = false; // toggle polygon mode
    //bool _ = false;

};

/*  Vertex is a point in 3D space
    pos: (x, y, z)
    color: (r, g, b)
    ...

*/
struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    int texIndex;

    static VkVertexInputBindingDescription getBindingDescription() {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
        std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

        attributeDescriptions[3].binding = 0;
        attributeDescriptions[3].location = 3;
        attributeDescriptions[3].format = VK_FORMAT_R32_SINT;
        attributeDescriptions[3].offset = offsetof(Vertex, texIndex);

        return attributeDescriptions;
    }

    bool operator==(const Vertex& other) const {
        return pos == other.pos && color == other.color && texCoord == other.texCoord;
    }
};

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^
                (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
                (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

struct RenderableData {
    std::vector<Vertex> vertices = {};
    std::vector<uint32_t> indices = {};
};

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};


struct Plane {
    std::vector<Vertex> vertices = {
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
    };

    std::vector<uint32_t> indices = {
        0,1,2, 0,2,3
    };
};

struct Cube {
    std::vector<Vertex> vertices = {
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },

        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },

        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 0.0f }, 0 },
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 0.0f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 1.0f }, 0 },
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 1.0f }, 0 },
    };

    std::vector<uint32_t> indices = {
        0,1,2, 0,2,3, 6,5,4,  7,6,4, 10,9,8, 11,10,8, 12,13,14, 12,14,15, 18,17,16, 19,18,16, 20,21,22, 20,22,23
    };
};


/* BASED ON SPECIFIC SKYBOX FILE USED:
total width: 4096px
total height: 3072px
letter width & height: 1024px
rows: 3
col: 4
*/
struct InvertedCube {
    std::vector<Vertex> vertices = {
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 6.f / 9.f }, 0 }, // BACK FACE
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.75f, 6.f / 9.f }, 0 },
        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.75f, 3.f / 9.f }, 0 },
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 1.0f, 3.f / 9.f }, 0 },

        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.75f, 3.f / 9.f }, 0 }, // RIGHT FACE
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 3.f / 9.f }, 0 },
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 6.f / 9.f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.75f, 6.f / 9.f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 6.f / 9.f }, 0 }, // FRONT FACE
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 6.f / 9.f }, 0 },
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 3.f / 9.f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 3.f / 9.f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 6.f / 9.f }, 0 }, // LEFT FACE
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 6.f / 9.f }, 0 }, 
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.0f, 3.f / 9.f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 3.f / 9.f }, 0 },

        { {  0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 0.0f }, 0 }, // TOP FACE
        { { -0.5,  0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 0.0f }, 0 },
        { { -0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 3.f / 9.f }, 0 },
        { {  0.5,  0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 3.f / 9.f }, 0 },

        { { -0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 6.f / 9.f }, 0 }, // BOTTOM FACE
        { {  0.5, -0.5, -0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 6.f / 9.f }, 0 },
        { {  0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.25f, 1.0f }, 0 },
        { { -0.5, -0.5,  0.5 }, {1.0, 1.0, 1.0}, { 0.5f, 1.0f }, 0 },
    };

    std::vector<uint32_t> indices = {
        2,1,0, 3,2,0, 4,5,6, 4,6,7, 8,9,10, 8,10,11, 14,13,12, 15,14,12, 16,17,18, 16,18,19, 22,21,20, 23,22,20
    };
};