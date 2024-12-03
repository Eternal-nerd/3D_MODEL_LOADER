#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "engine.h"

// Let the game begin
int main() {
  Engine eng;

  try {
    eng.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
