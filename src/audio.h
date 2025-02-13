#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <vector>
#include <string>

#include "util.h"
#include "types.h"

class Audio {
public:
	Audio();
	~Audio();

	void init();

	// FIXME TESTING
	void play(int soundIndex);

	void cleanup();

private:
	std::vector<std::string> audioFilenames_ = {};

	SDL_AudioStream* stream_ = NULL;
	Uint8* wavData_ = NULL;
	Uint32 wavDataLen_ = 0;
	SDL_AudioSpec audioSpec_;

};