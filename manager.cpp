#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include "vector2f.h"
#include "twowaymultisprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
#include "player.h"
#include "aaline.h"
#include "sound.h"


class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite* lhs, const ScaledSprite* rhs) {
    return lhs->getScale() < rhs->getScale();
  }
};

Manager::~Manager() { 

  std::vector<ScaledSprite*>::const_iterator ptrPainterSprites = painterSprites.begin();
  while (ptrPainterSprites != painterSprites.end()) {
    delete (*ptrPainterSprites);
    ++ptrPainterSprites;
  }
  std::vector<Sprite*>::const_iterator ptrEnemies = enemies.begin();
  while (ptrEnemies != enemies.end()) {
    delete (*ptrEnemies);
    ++ptrEnemies;
  }
  std::vector<ExplodingSprite*>::const_iterator ptrExplodingEnemies = explodingEnemies.begin();
  while (ptrExplodingEnemies != explodingEnemies.end()) {
    delete (*ptrExplodingEnemies);
    ++ptrExplodingEnemies;
  }

  std::vector<Sprite*>::const_iterator ptrKilledEnemies = killedEnemies.begin();
  while (ptrKilledEnemies != killedEnemies.end()) {
    delete (*ptrKilledEnemies);
    ++ptrKilledEnemies;
  }

  std::vector<SmartEnemy*>::const_iterator ptrSmartEnemies = smartEnemies.begin();
  while (ptrSmartEnemies != smartEnemies.end()) {
    delete (*ptrSmartEnemies);
    ++ptrSmartEnemies;
  }

  std::vector<SmartEnemy*>::const_iterator ptrKilledSmartEnemies = killedSmartEnemies.begin();
  while (ptrKilledSmartEnemies != killedSmartEnemies.end()) {
    delete (*ptrKilledSmartEnemies);
    ++ptrKilledSmartEnemies;
  }

  painterSprites.clear();
  enemies.clear();
  explodingEnemies.clear();
  killedEnemies.clear();
  smartEnemies.clear();
  killedSmartEnemies.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  hud(Hud::getInstance()),
  screen( io.getScreen() ),
  world1("back1", Gamedata::getInstance().getXmlInt("back1/factor") ),
  world2("back2", Gamedata::getInstance().getXmlInt("back2/factor")),
  viewport( Viewport::getInstance() ),
  player(Player::getInstance()),
  currentSprite(),
  reset(false),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  painterSprites(),
  enemies(),
  killedEnemies(),
  smartEnemies(),
  killedSmartEnemies(),
  explodingEnemies(),
  health(Health::getInstance()),
  hudConclusion(HudConclusion::getInstance())
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);


  viewport.setObjectToTrack(&player);
  
  unsigned numberOfpainterSprites = Gamedata::getInstance().getXmlInt("numberOfpainterSprites");
    painterSprites.reserve(numberOfpainterSprites);
    for (unsigned i = 0; i < numberOfpainterSprites; ++i) {
      painterSprites.push_back(new ScaledSprite("painterSprite"));
    }
    sort(painterSprites.begin(), painterSprites.end(), ScaledSpriteCompare());

    unsigned numberOfenemies = Gamedata::getInstance().getXmlInt("numberOfenemies");
    enemies.reserve(numberOfenemies);
    for (unsigned i = 0; i < numberOfenemies; ++i) {
      enemies.push_back(new Sprite("enemy"));
    }

    unsigned numberOfSmartEnemies = Gamedata::getInstance().getXmlInt("numberOfsmartenemies");
    smartEnemies.reserve(numberOfSmartEnemies);
    for (unsigned i = 0; i < numberOfSmartEnemies; ++i) {
      smartEnemies.push_back(new SmartEnemy("smartEnemy"));
    }

  }

void Manager::draw() const {
  world1.draw();
  for (unsigned i = 0; i < painterSprites.size(); ++i) {
    painterSprites[i]->draw();
  }
  world2.draw();
  for (unsigned i = 0; i < enemies.size(); ++i) {
    enemies[i]->draw();
  }
  for (unsigned i = 0; i < explodingEnemies.size(); ++i)
  {
    explodingEnemies[i]->draw();
  }
  for (unsigned i = 0; i < smartEnemies.size(); ++i)
  {
    smartEnemies[i]->draw();
  }
  player.draw();
  IOManager::getInstance().printMessageCenteredAt(title, 30);
  if(player.getGodMode())
    IOManager::getInstance().printMessageAt(Gamedata::getInstance().getXmlStr("godMode/message"), Gamedata::getInstance().getXmlInt("godMode/locX"), Gamedata::getInstance().getXmlInt("godMode/locY"));

  if (hud.checkHUDVisible())
  {
    hud.setScore(killedEnemies.size()+ killedSmartEnemies.size());
    hud.setLives(player.getLives());
    hud.setHealth(player.getHealth());

    hud.drawHUD(screen);
    hud.draw();
  }

  if (enemies.empty() && smartEnemies.empty() && player.getLives()>=0)
  {
    hudConclusion.setWon("You Won!!");
    hudConclusion.setScore(killedEnemies.size() + killedSmartEnemies.size());
    hudConclusion.drawHUD(screen);
    hudConclusion.draw();
  }

  if (player.getLives()<0 && !(enemies.empty() && smartEnemies.empty()))
  {
    hudConclusion.setWon("You Lost!!");
    hudConclusion.setScore(killedEnemies.size() + killedSmartEnemies.size());
    hudConclusion.drawHUD(screen);
    hudConclusion.draw();
  }

  viewport.draw();

  SDL_Flip(screen);
}

// Move this to IOManager
void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}


void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }
  world1.update();
  world2.update();
  if (player.getHealth() == 0)
  {
    player.explode();
    player.resetHealth();
    player.reduceLives();
  }

  player.update(ticks);
  for (unsigned i = 0; i < painterSprites.size(); ++i) {
    painterSprites[i]->update(ticks);
  }
  for (unsigned i = 0; i < enemies.size(); ++i) {
    enemies[i]->update(ticks);
  }
  for (unsigned i = 0; i < smartEnemies.size(); ++i)
  {
    smartEnemies[i]->update(ticks);
  }
  std::vector<ExplodingSprite*>::iterator ptrExplodingEnemies = explodingEnemies.begin();
  while (ptrExplodingEnemies != explodingEnemies.end()) {
    (*ptrExplodingEnemies)->update(ticks);
    if ((*ptrExplodingEnemies)->chunkCount() == 0)
    {
      delete (*ptrExplodingEnemies);
      ptrExplodingEnemies = explodingEnemies.erase(ptrExplodingEnemies);
    }
    else
      ++ptrExplodingEnemies;
  }

  checkForBulletCollisions();
  checkPlayerGermCollisions();
  health.update(ticks);
  viewport.update(); // always update viewport last
}

void Manager::doreset()
{
  if (killedEnemies.size() > 0)
  {
    std::vector<Sprite*>::iterator ptrKilledEnemies = killedEnemies.begin();
    while (ptrKilledEnemies != killedEnemies.end()) {
      enemies.push_back(*ptrKilledEnemies);
      ptrKilledEnemies = killedEnemies.erase(ptrKilledEnemies);
    }
  }
  killedEnemies.clear();

  if (killedSmartEnemies.size() > 0)
  {
    std::vector<SmartEnemy*>::iterator ptrKilledSmartEnemies = killedSmartEnemies.begin();
    while (ptrKilledSmartEnemies != killedSmartEnemies.end()) {
      smartEnemies.push_back(*ptrKilledSmartEnemies);
      ptrKilledSmartEnemies = killedSmartEnemies.erase(ptrKilledSmartEnemies);
    }
  }
  killedSmartEnemies.clear();

  std::vector<ExplodingSprite*>::const_iterator ptrExplodingEnemies = explodingEnemies.begin();
  while (ptrExplodingEnemies != explodingEnemies.end()) {
    delete (*ptrExplodingEnemies);
    ++ptrExplodingEnemies;
  }
  
  explodingEnemies.clear();
}

void Manager::play() {
  SDL_Event event;
  SDLSound sound;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_s]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
        if (event.key.keysym.sym == SDLK_a)
        {
          player.Left();

        }
        if (keystate[SDLK_d])
        {
          player.Right();

        }
        if (event.key.keysym.sym == SDLK_s)
        {
          player.Down();

        }
        if (keystate[SDLK_w])
        {
          player.Up();
        }
        if (keystate[SDLK_F1])
        {
          if(!hud.isFirstLoad)
            hud.isHUDVisible = !hud.isHUDVisible;
          if (hud.isFirstLoad)
            hud.isFirstLoad = !hud.isFirstLoad;
        }
        
        if (keystate[SDLK_r])
        {
          player.reset();
          hud.reset();
          clock.reset();
          doreset();
        }
        if (keystate[SDLK_SPACE])
        {
          player.shoot();
          sound[0];
        }
        if (keystate[SDLK_g])
        {
          player.setGodMode();
        }
      }
      //else if(event.type == SDL_PRESSED && event.type == SDL_KEYUP )
      else if ( event.type == SDL_KEYUP)
      {
       if (keystate[SDLK_a])
        {
          player.Left();

        }
        else if (keystate[SDLK_d])
        {
          player.Right();

        }
        else if (keystate[SDLK_s])
        {
          player.Down();

        }
        else if (keystate[SDLK_w])
        {
          player.Up();

        }
       else 
       {
        player.Stop();
       }

      }
      else
      {
        player.Stop();
      }
    }
    draw();
    update();
  }
}

void Manager::checkForBulletCollisions() {
  std::vector<Sprite*>::iterator ptrEnemies = enemies.begin();
  while (ptrEnemies != enemies.end()) {
    if (player.checkForBulletCollisions(*ptrEnemies))
    {
      explodingEnemies.push_back(new ExplodingSprite(*ptrEnemies));
      killedEnemies.push_back(*ptrEnemies);
      ptrEnemies = enemies.erase(ptrEnemies);
      return;
    }
    else
    ++ptrEnemies;
  }

  std::vector<SmartEnemy*>::iterator ptrSmartEnemies = smartEnemies.begin();
  while (ptrSmartEnemies != smartEnemies.end()) {
    if (player.checkForBulletCollisions(*ptrSmartEnemies))
    {
      explodingEnemies.push_back(new ExplodingSprite(*ptrSmartEnemies));
      killedSmartEnemies.push_back(*ptrSmartEnemies);
      ptrSmartEnemies = smartEnemies.erase(ptrSmartEnemies);
      return;
    }
    else
      ++ptrSmartEnemies;
  }

}

void Manager::checkPlayerGermCollisions() {
  std::vector<Sprite*>::iterator ptrEnemies = enemies.begin();
  while (ptrEnemies != enemies.end()) {
    if (player.collidedWith(*ptrEnemies))
    {
      explodingEnemies.push_back(new ExplodingSprite(*ptrEnemies));
      killedEnemies.push_back(*ptrEnemies);
      ptrEnemies = enemies.erase(ptrEnemies);
      if(!player.getGodMode())
        player.reduceHealth();
      return;
    }
    else
      ++ptrEnemies;
  }
  
  std::vector<SmartEnemy*>::iterator ptrSmartEnemies = smartEnemies.begin();
  while (ptrSmartEnemies != smartEnemies.end()) {
    if (player.collidedWith(*ptrSmartEnemies))
    {

      explodingEnemies.push_back(new ExplodingSprite(*ptrSmartEnemies));
      killedSmartEnemies.push_back(*ptrSmartEnemies);
      ptrSmartEnemies = smartEnemies.erase(ptrSmartEnemies);
      if (!player.getGodMode())
        player.reduceHealth();
      
      return;

    }
    else
      ++ptrSmartEnemies;
  }

}