#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
