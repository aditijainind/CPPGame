#include "health.h"

Health& Health::getInstance() {
  static Health health;
  return health;
}

Health::Health() :
  start(Vector2f(Gamedata::getInstance().getXmlInt("health/locX"), Gamedata::getInstance().getXmlInt("health/locY"))),
  totalLength(Gamedata::getInstance().getXmlInt("health/length")),
  currentLength(Gamedata::getInstance().getXmlInt("health/length")),
  thick(Gamedata::getInstance().getXmlInt("health/thickness")),
  increments(Gamedata::getInstance().getXmlInt("health/increment")),
  interval(Gamedata::getInstance().getXmlInt("health/interval")),
  deltaTime(Gamedata::getInstance().getXmlInt("health/deltaTime")) {
}

Health::Health(int sx, int sy, int tl, int cl,
               float t, int inc, float sp):
  start(Vector2f(sx, sy)), 
  totalLength(tl), 
  currentLength(cl), 
  thick(t), 
  increments(inc),
  interval(sp),
  deltaTime(SDL_GetTicks())
{
}

void Health::drawBox(SDL_Surface* screen) const {
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+totalLength, start[1], 
                      thick, SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) );
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8, 
                      start[0]+totalLength, start[1]-8, 
                      1.0, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
  Draw_AALine(screen, start[0], start[1]+8, 
                      start[0]+totalLength, start[1]+8, 
                      1.0, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8, 
                      start[0]-1, start[1]+8, 
                      2.0, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8, 
                      start[0]+totalLength+1, start[1]+8, 
                      2.0, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
}

void Health::draw(SDL_Surface* screen) const {
  drawBox(screen);
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+currentLength, start[1], 
                      thick, SDL_MapRGB(screen->format, 0xff, 0x00, 0x00));
}
void Health::update(Uint32 ticks) {
  deltaTime += ticks;
  if ( currentLength > 0 && deltaTime > interval ) {
    deltaTime = 0;
    currentLength -= increments;
  }
}
