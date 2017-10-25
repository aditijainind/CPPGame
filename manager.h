#include <list>
#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "world.h"
#include "viewport.h"
#include "scaledSprite.h"
#include "player.h"
#include "hud.h"
#include "health.h"
#include "smartEnemy.h"
#include "hudConclusion.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();
  void drawHUD(SDL_Surface*, int, int) const;
  void checkForBulletCollisions();
  void checkPlayerGermCollisions();
  bool Reset() { return reset; }
  void doreset();
private:
  const bool env;
  const IOManager& io;
  Clock& clock;
  Hud& hud;
  SDL_Surface * const screen;
  World world1;
  World world2;

  Viewport& viewport;

  Player& player;
  std::list<Drawable*>::iterator currentSprite;
 
  bool reset;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
  std::vector<ScaledSprite*> painterSprites;
  std::vector<Sprite*> enemies;
  std::vector<Sprite*> killedEnemies;
  std::vector<SmartEnemy*> smartEnemies;
  std::vector<SmartEnemy*> killedSmartEnemies;
  std::vector<ExplodingSprite*> explodingEnemies;
  Health& health;
  HudConclusion& hudConclusion;

};
