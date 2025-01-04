#pragma once

#include "dvce.h"
#include "util.h"

class Gfx {
public:
	Gfx();
	~Gfx();

	void init();

	void drawRenderable();

	void cleanup();

	void setWindowPtr(SDL_Window* window);


private:
	// SDL WINDOW
	SDL_Window* window_ = nullptr;

	Dvce dvce_;

};