#pragma once

#include "types.h"

// CONFIGURATION
const uint32_t WIDTH = 300;
const uint32_t HEIGHT = 200;

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation" };

// INPUT DATA
const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 1.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, 1.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 1.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}};

const std::vector<uint16_t> input_indices = {
    0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4, 4, 1, 2, 6, 3, 0 // FIXME
};

/*const std::vector<Vertex> vertices = {
    {{-0.5f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // bottom left 0
    {{0.5f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // bottom right 1
    {{0.5f, 1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},   // top right 2
    {{-0.5f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // top left 3
    {{-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 1.0f}},  // middle left 4
    {{1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}};  // middle right 5*/

// const std::vector<uint16_t> input_indices = { 0, 1, 2, 2, 3, 0 };
