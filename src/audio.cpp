#include "audio.h"

Audio::Audio() {}
Audio::~Audio() {}


/*-----------------------------------------------------------------------------
------------------------------INITIALIZATION-----------------------------------
-----------------------------------------------------------------------------*/
void Audio::init() {
	util::log("Initializing audio object...");

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

    // for now, create SDL audio stream on init, and set audio spec to first file in list..
    if (!SDL_LoadWAV(audioFilenames_[0].c_str(), &audioSpec_, &wavData_, &wavDataLen_)) {
        throw std::runtime_error("Failed to load .WAV file: " + audioFilenames_[0]);
    }

    /* Create our audio stream in the same format as the .wav file. It'll convert to what the audio hardware wants. */
    stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &audioSpec_, NULL, NULL);
    if (!stream_) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        throw std::runtime_error("Failed to create SDL audio stream! ");
    }

    /* SDL_OpenAudioDeviceStream starts the device paused. You have to tell it to start! */
    SDL_ResumeAudioStreamDevice(stream_);
}


/*-----------------------------------------------------------------------------
------------------------------PLAY-SOUNDS--------------------------------------
-----------------------------------------------------------------------------*/
void Audio::play(int soundIndex) {
    //util::log("Playing sound with index=" + std::to_string(soundIndex));

    // free PCM data previously loaded into buffer
    SDL_free(wavData_);

    if (!SDL_LoadWAV(audioFilenames_[soundIndex].c_str(), &audioSpec_, &wavData_, &wavDataLen_)) {
        throw std::runtime_error("Failed to load .WAV file: " + audioFilenames_[soundIndex]);
    }

    // clear audiostream incase of past sounds still replaying:
    // FIXMEEEEE need to add overlapping sounds
    SDL_ClearAudioStream(stream_);

    /* feed more data to the stream. It will queue at the end, and trickle out as the hardware needs more data. */
    SDL_PutAudioStreamData(stream_, wavData_, wavDataLen_);
}


/*-----------------------------------------------------------------------------
------------------------------CLEANUP------------------------------------------
-----------------------------------------------------------------------------*/
void Audio::cleanup() {
	util::log("Destroying audio object...");
    
    // anything to do here?
    SDL_free(wavData_);
    
}
