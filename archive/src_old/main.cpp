#include <cstdlib>
#include <iostream>

#include <SDL3/SDL_main.h>

#include "engine.h"

int main() {
  try {
    std::cout << "HELLO WORLT. ;)\n";
    Engine eng;
    eng.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
