#ifndef PLAYER_H
#define PLAYER_H

#include "twowaymultisprite.h"
#include <string>
#include "ioManager.h"
#include "explodingSprite.h"
#include "projectilePool.h"
class Player :public TwoWayMultiSprite
{
public:
  static Player& getInstance();

  virtual ~Player();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  
  void Stop();
  void Up();
  void Down();
  void Left();
  void Right();
  void explode();
  void shoot();
  void reset();

  bool collidedWith(const Drawable* d) const;

  bool checkForBulletCollisions(const Drawable* d);
  void setGodMode();
  bool getGodMode();
  void reduceHealth();
  int getHealth();
  void resetHealth();
  void reduceLives();
  int getLives();
  void resetLives();
private:
  Player(const std::string&, const std::string&);
  Player(const Player&);
  Player& operator=(const Player&);

  ExplodingSprite* explosion;
  
  Vector2f playerVelocity;
  Vector2f startPos;
  bool isStopped;
  bool isShooting;
  float velocityXFromXml;
  float velocityYFromXml;
  float minimumX;
  float minimumY;
  float maximumX;
  float maximumY;
  ProjectilePool projectilePool;
  CollisionStrategy * strategy;
  int initialHealth;
  int playerHealth;
  int initialLives;
  int playerLives;
  bool isGodMode;
};

#endif
