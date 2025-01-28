#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NO_INCLUDE_STB_IMAGE 
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.

#include "engine.h"

// Program entry point
int main(int argv, char **args) {
  Engine eng;

  try {
    eng.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
