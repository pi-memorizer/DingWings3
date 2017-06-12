#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H
#include "Revengine.h"

class Sound;

class SoundSystem
{
	string song;
	Mix_Music *music;
public:
	SoundSystem();
	~SoundSystem();
	void setBackgroundMusic(string name);
};

class Sound
{
	Mix_Chunk *chunk;
public:
	Sound(string name);
	~Sound();
	void play();
};

extern SoundSystem *soundSystem;
extern Sound *sounds[];

#endif