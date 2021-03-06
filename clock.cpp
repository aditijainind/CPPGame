#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include "clock.h"
#include "gamedata.h"
#include "ioManager.h"

Clock& Clock::getInstance() {
  if ( SDL_WasInit(SDL_INIT_VIDEO) == 0) {
    throw std::string("Must init SDL before Clock");
  }
  static Clock clock; 
  return clock;
}

Clock::Clock() :
  ticks(0),
  ticksSinceReset(0),
  requiredTicksBetweenFrames(1000/ Gamedata::getInstance().getXmlInt("frameCap") + 1),
  totalTicks(0),
  started(false), 
  paused(false), 
  sloMo(false), 
  sumOfTicks(SDL_GetTicks()),
  pos( Gamedata::getInstance().getXmlInt("clock/locX"),
       Gamedata::getInstance().getXmlInt("clock/locY"))
  {
  start();
}

void Clock::draw() const { 
  IOManager::getInstance().
    printMessageValueAt("Seconds: ", getSeconds(), pos[0], pos[1]);
  IOManager::getInstance().
    printMessageValueAt("fps: ", getFps(), pos[0], pos[1]+20);
}

void Clock::update() { 
  if (!paused)
  {
    if (totalTicks - sumOfTicks < requiredTicksBetweenFrames)
      SDL_Delay(requiredTicksBetweenFrames - totalTicks + sumOfTicks);
    totalTicks = SDL_GetTicks() - ticksSinceReset;
    ticks = totalTicks - sumOfTicks;
    sumOfTicks += ticks;
  }
  else
  {
    ticks = 0;
    sumOfTicks = SDL_GetTicks();
  }
}

unsigned int Clock::getTicksSinceLastFrame() const {
  if (!sloMo)
    return ticks;
  return ticks / 3;
}

void Clock::reset()
{
  ticks = 0;
  totalTicks = 0;
  ticksSinceReset += sumOfTicks;
  sumOfTicks = 0;
}

void Clock::toggleSloMo() {
  sloMo = ! sloMo;
}

int Clock::getFps() const { 
  if ( ticks > 0 ) return 1000/ticks;
  return 0;
}

void Clock::start() { 
  started = true; 
  paused = false; 
}

void Clock::pause() { 
  paused = true;
}

void Clock::unpause() { 
  paused = false;
}

