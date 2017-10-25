#ifndef HUDCONCLUSION__H
#define HUDCONCLUSION__H

#include "aaline.h"
#include "vector2f.h"
#include "gamedata.h"
#include "ioManager.h"
#include <SDL.h>
#include <string>
class HudConclusion
{
public:
  static HudConclusion& getInstance();
  void drawHUD(SDL_Surface*) const;
  void draw() const;
  void writeHudConclusionMessage(const std::string) const;
  void setBullets(unsigned int b) { bulletList = b; }
  void setFree(unsigned int f) { freeList = f; }
  void setScore(unsigned int s) { score = s; }
  void setWon(const std::string w) { won = w; }
private:
  const Vector2f position;
  const int width;
  const int height;
  unsigned int bulletList;
  unsigned int freeList;
  unsigned int score;
  std::string won;
  HudConclusion(double, double, double, double);
  HudConclusion();
  HudConclusion(const HudConclusion&);
  HudConclusion& operator=(const HudConclusion&);

};

#endif
