#include "engine.h"

#include <iostream>

Engine::Engine(SDL_Window *win) : win_(win) { init(); }

Engine::~Engine() {}

void Engine::init() { std::cout << "init engine \n"; }
