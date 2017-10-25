#include "hud.h"
 
Hud& Hud::getInstance()
{
  static Hud hud;
  return hud;
}

Hud::Hud() :
  isHUDVisible(false),
  position(Vector2f(Gamedata::getInstance().getXmlInt("hud/locX"), Gamedata::getInstance().getXmlInt("hud/locY"))),
  width(Gamedata::getInstance().getXmlInt("hud/width")),
  height(Gamedata::getInstance().getXmlInt("hud/height")),
  clock(Clock::getInstance()),
  bulletList(0),
  freeList(0),
  score(0),
  isFirstLoad((clock.getSeconds() <= (unsigned int)Gamedata::getInstance().getXmlInt("hud/hudDuration"))),
  lives(0),
  health(0)
{}

void Hud::drawHUD(SDL_Surface* screen) const
{
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, position[0], position[1]+ height / 2, position[0]+ width, position[1]+ height / 2, height, 0xff, 0xff, 0xff, 0xff / 2);
  Draw_AALine(screen, position[0], position[1], position[0] + width, position[1], RED);
}

void Hud::draw() const
{
  clock.draw();
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text1"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 4* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text2"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 6* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text3"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 8* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text4"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 10* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text5"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 12* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text7"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 14 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hud/text8"), position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 16 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageValueAt("Bullet List: ", bulletList, position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 18 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageValueAt( "Free List: ", freeList, position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 20* Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageValueAt("Score: ", score, position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 22 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageValueAt("Health: ", health, position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 24 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
  IOManager::getInstance().printMessageValueAt("Lives: ", lives, position[0] + Gamedata::getInstance().getXmlInt("hud/intervalX"), position[1] + 26 * Gamedata::getInstance().getXmlInt("hud/intervalY"));
} 

void Hud::writeHudMessage(std::string message) const
{
  IOManager::getInstance().printMessageAt(message, position[0], position[1]);
}

bool Hud::checkHUDVisible() 
{
  if (isFirstLoad)
    isFirstLoad = clock.getSeconds() <= (unsigned int)Gamedata::getInstance().getXmlInt("hud/hudDuration");
  return isFirstLoad | isHUDVisible;
}

void Hud::reset()
{ 
  isHUDVisible = false; 
  isFirstLoad = true; 
}
