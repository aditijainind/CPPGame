#include <string>
#include <iostream>
#include <ctime>
#include "sound.h"
#include "gamedata.h"

using std::string;

SDLSound::~SDLSound() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
  for (unsigned int i = 0; i < sounds.size(); ++i) {
    Mix_FreeChunk(sounds[i]);
  }
  Mix_CloseAudio();

}

SDLSound::SDLSound() : 
  volume(SDL_MIX_MAXVOLUME/4), 
  currentSound(-1), 
  music(NULL),
  audioRate(22050), 
  audioChannels(2), 
  audioBuffers(4096),
  sounds(),
  channels() 
{
  if(Mix_OpenAudio(audioRate, MIX_DEFAULT_FORMAT, audioChannels, 
                   audioBuffers)){
    throw string("Unable to open audio!");
  }
  music = Mix_LoadMUS((Gamedata::getInstance().getXmlStr("sound/background")).c_str());
  if (!music) throw string("Couldn't load background")+Mix_GetError();

  startMusic();
  sounds.push_back( Mix_LoadWAV((Gamedata::getInstance().getXmlStr("sound/bullet")).c_str()));

  for (unsigned int i = 0; i < sounds.size(); ++i) channels.push_back(i);
}

void SDLSound::toggleMusic() {
  if( Mix_PausedMusic() ) { 
    Mix_ResumeMusic(); 
  } 
  else { 
    Mix_PauseMusic(); 
  } 
}

void SDLSound::operator[](int index) {
  if (currentSound >= 0) Mix_HaltChannel(currentSound);
  currentSound = index;
  Mix_VolumeChunk(sounds[index], volume);
  channels[index] = 
     Mix_PlayChannel(channels[index], sounds[index], 0);
}

void SDLSound::startMusic() {
  Mix_VolumeMusic(volume);
  Mix_PlayMusic(music, -1);
}

void SDLSound::stopMusic() {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

