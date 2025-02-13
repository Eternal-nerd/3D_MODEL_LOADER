#pragma once

#include <AL/al.h>
#include <AL/alc.h>
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

	// OpenAL stuff
	ALCdevice* device_ = nullptr;
	ALCcontext* context_ = nullptr;

	// buffer
	ALuint audioBuffer_;
	// single source for now:
	ALuint audioSource_;

	//err
	ALenum err_;


	// util
	bool loadWAV(const std::string& filename, std::uint8_t& channels, std::int32_t& sample_rate, std::uint8_t& bits_per_sample, std::vector<char>& sound_data);


};