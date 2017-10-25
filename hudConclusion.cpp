#include "hudConclusion.h"
 
HudConclusion& HudConclusion::getInstance()
{
  static HudConclusion hudConclusion;
  return hudConclusion;
}

HudConclusion::HudConclusion() :
  position(Vector2f(Gamedata::getInstance().getXmlInt("hudConclusion/locX"), Gamedata::getInstance().getXmlInt("hudConclusion/locY"))),
  width(Gamedata::getInstance().getXmlInt("hudConclusion/width")),
  height(Gamedata::getInstance().getXmlInt("hudConclusion/height")),
  bulletList(0),
  freeList(0),
  score(0),
  won("")
{}

void HudConclusion::drawHUD(SDL_Surface* screen) const
{
  const Uint32 RED = SDL_MapRGB(screen->format, 0xff, 0, 0);
  Draw_AALine(screen, position[0], position[1]+ height / 2, position[0]+ width, position[1]+ height / 2, height, 0xff, 0xff, 0xff, 0xff / 2);
  Draw_AALine(screen, position[0], position[1], position[0] + width, position[1], RED);
}

void HudConclusion::draw() const
{
  IOManager::getInstance().printMessageAt(won, position[0] + Gamedata::getInstance().getXmlInt("hudConclusion/intervalX"), position[1] + 2 * Gamedata::getInstance().getXmlInt("hudConclusion/intervalY"));
  IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("hudConclusion/text2"), position[0] + Gamedata::getInstance().getXmlInt("hudConclusion/intervalX"), position[1] + 6* Gamedata::getInstance().getXmlInt("hudConclusion/intervalY"));
  IOManager::getInstance().printMessageValueAt("Score: ", score, position[0] + Gamedata::getInstance().getXmlInt("hudConclusion/intervalX"), position[1] + 8 * Gamedata::getInstance().getXmlInt("hudConclusion/intervalY"));
} 

void HudConclusion::writeHudConclusionMessage(std::string message) const
{
  IOManager::getInstance().printMessageAt(message, position[0], position[1]);
}


