#include "audio.h"

Audio::Audio() {}
Audio::~Audio() {}


/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Audio::init() {
	util::log("Initializing audio object...");

    device_ = alcOpenDevice(NULL);
    if (device_ == NULL) {
        throw std::runtime_error("Failed to open default sound device. ");
    }

    context_ = alcCreateContext(device_, NULL);
    if (context_ == NULL) {
        throw std::runtime_error("Failed to create ALC context. ");
    }

    alcMakeContextCurrent(context_);

    // load audio files
    audioFilenames_ = {
        "../res/audio/wav/jump.wav",
        "../res/audio/wav/land.wav",
        "../res/audio/wav/step1.wav",
        "../res/audio/wav/step2.wav",
        "../res/audio/wav/step3.wav",
        "../res/audio/wav/step4.wav",
        "../res/audio/wav/step5.wav",
    };
}

bool Audio::loadWAV(const std::string& filename, std::uint8_t& channels, std::int32_t& sample_rate, std::uint8_t& bits_per_sample, std::vector<char>& sound_data) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    ALsizei size;
    char buffer[4];

    // RIFF
    file.read(buffer, 4);

    // size of file 
    file.read(buffer, 4);

    // WAVE 
    file.read(buffer, 4);

    // fmt/0
    file.read(buffer, 4);
    file.read(buffer, 4);

    file.read(buffer, 2); //  , "Core::Audio::LoadWAV(): could not read audio format (1 for PCM -> liniar quantization)");

    // channels
    file.read(buffer, 2);
    channels = util::convertToInt(buffer, 2);

    // sample rate
    file.read(buffer, 4);
    sample_rate = util::convertToInt(buffer, 4);

    file.read(buffer, 4);

    file.read(buffer, 2);

    file.read(buffer, 2);
    bits_per_sample = util::convertToInt(buffer, 2);

    // data subchunk
    file.read(buffer, 4);
    //printf("-----|%s| \n\n", buffer);

    file.read(buffer, 4);
    size = util::convertToInt(buffer, 4);

    file.eof();

    char* c = new char[size];
    file.read(c, size);

    sound_data.resize(size);
    for (int i = 0; i < size; i++) {
        sound_data[i] = c[i];
    }

    return true;
}

/*-----------------------------------------------------------------------------
------------------------------PLAY-SOUNDS--------------------------------------
-----------------------------------------------------------------------------*/
void Audio::play(int soundIndex) {
    //util::log("Playing sound with index=" + std::to_string(soundIndex));

    std::uint8_t channels;
    std::int32_t sample_rate;
    std::uint8_t bits_per_sample;
    std::vector<char> sound_data;

    if (!loadWAV(audioFilenames_[soundIndex], channels, sample_rate, bits_per_sample, sound_data)) {
        throw std::runtime_error("Failed to load .WAV file: " + audioFilenames_[soundIndex]);
    }

    // generate AL buffers
    alGetError(); // clear error

    // might need to do something with channels here?
    alGenBuffers(1, &audioBuffer_);

    err_ = alGetError();
    if (err_ != AL_NO_ERROR) {
        throw std::runtime_error("Failed to generate AL buffer. ");
    }

    // format of sound:
    ALenum format;
    if(channels == 1 && bits_per_sample == 8) {
        format = AL_FORMAT_MONO8;
    }
    else if(channels == 1 && bits_per_sample == 16) {
        format = AL_FORMAT_MONO16;
    }
    else if(channels == 2 && bits_per_sample == 8) {
        format = AL_FORMAT_STEREO8;
    }
    else if(channels == 2 && bits_per_sample == 16) {
        format = AL_FORMAT_STEREO16;
    }
    else {
        throw std::runtime_error("attempting to playback an unknown audio file format. ");
    }

    // copy sound data into buffer for playback
    alBufferData(audioBuffer_, format, sound_data.data(), sound_data.size(), sample_rate);
    err_ = alGetError();
    if (err_ != AL_NO_ERROR) {
        throw std::runtime_error("Failed to copy sound data into AL buffer for playback. ");
    }
    sound_data.clear();

    // generate audio source, simple for now:
    alGenSources(1, &audioSource_);
    alSourcef(audioSource_, AL_PITCH, 1);
    alSourcef(audioSource_, AL_GAIN, 1.0f);
    alSource3f(audioSource_, AL_POSITION, 0, 0, 0);
    alSource3f(audioSource_, AL_VELOCITY, 0, 0, 0);
    alSourcei(audioSource_, AL_LOOPING, AL_FALSE);
    alSourcei(audioSource_, AL_BUFFER, audioBuffer_);

    alSourcePlay(audioSource_);
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Audio::cleanup() {
	util::log("Destroying audio object...");

    alDeleteSources(1, &audioSource_);
    alDeleteBuffers(1, &audioBuffer_);
    alcDestroyContext(context_);
    alcCloseDevice(device_);
}
