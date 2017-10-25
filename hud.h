#ifndef HUD__H
#define HUD__H

#include "aaline.h"
#include "vector2f.h"
#include "gamedata.h"
#include "ioManager.h"
#include <SDL.h>
#include <string>
#include "clock.h"
class Hud
{
public:
  static Hud& getInstance();
  void drawHUD(SDL_Surface*) const;
  void draw() const;
  void writeHudMessage(const std::string) const;
  bool checkHUDVisible(); 
  bool isHUDVisible;
  void setBullets(unsigned int b) { bulletList = b; }
  void setFree(unsigned int f) { freeList = f; }
  void setScore(unsigned int s) { score = s; }
  void setLives(unsigned int l) { lives = l; }
  void setHealth(unsigned int h) { health = h; }
  void reset();
private:
  const Vector2f position;
  const int width;
  const int height;
  Clock& clock;
  unsigned int bulletList;
  unsigned int freeList;
  unsigned int score;

  Hud(double, double, double, double);
  Hud();
  Hud(const Hud&);
  Hud& operator=(const Hud&);
public:
  bool isFirstLoad;
  unsigned int lives;
  unsigned int health;
};

#endif
