#include <iostream>
#include <cmath>
#include "projectilePool.h"
#include "ioManager.h"
#include "hud.h"
ProjectilePool::ProjectilePool(const string& name) :
  projectileFrame(FrameFactory::getInstance().getFrame(name+"Bullet")),
  frameInterval(Gamedata::getInstance().getXmlInt(name + "Bullet/Interval")),
  ticksSinceLastFrame(0),
  projectiles(),
  freeList() {
  
}

ProjectilePool::~ProjectilePool() {

  std::list<Projectile*>::const_iterator ptrProjectiles = projectiles.begin();
  while (ptrProjectiles != projectiles.end()) {
    delete (*ptrProjectiles);
    ++ptrProjectiles;
  }
  std::list<Projectile*>::const_iterator ptrFreeList = freeList.begin();
  while (ptrFreeList != freeList.end()) {
    delete (*ptrFreeList);
    ++ptrFreeList;
  }

  projectiles.clear(); 
  freeList.clear();   
}

void ProjectilePool::draw() const {

  Hud::getInstance().setBullets(projectiles.size());
  Hud::getInstance().setFree(freeList.size());
  std::list<Projectile*>::const_iterator ptr = projectiles.begin();
  while (ptr != projectiles.end()) {
    (*ptr)->draw();
    ++ptr;
  }
}

void ProjectilePool::update(Uint32 ticks) {
  ticksSinceLastFrame += ticks;
  std::list<Projectile*>::iterator ptr = projectiles.begin();
  while (ptr != projectiles.end()) {
    (*ptr)->update(ticks);
    if ((*ptr)->goneTooFar()) {  
      freeList.push_back(*ptr);
      ptr = projectiles.erase(ptr);
    }
    else ++ptr;
  }
}

void ProjectilePool::shoot(const Vector2f& position, const Vector2f& velocity, const string& name) {
  if(ticksSinceLastFrame>frameInterval)
  {
  if (freeList.empty()) {
    Projectile* p = new Projectile(
      position+Vector2f(0,50), 
      velocity,
      name, projectileFrame);
    projectiles.push_back(p);
  }
  else {
    Projectile* p = freeList.front();
    freeList.pop_front();
    p->reset();
    p->setVelocity(velocity);
    p->velocityY(0);
    p->setPosition(position+Vector2f(0,50));
    projectiles.push_back(p);
  }
  ticksSinceLastFrame=0;
 }
}
bool ProjectilePool::checkForBulletCollisions(const Drawable* d) 
{
  std::list<Projectile*>::iterator ptr = projectiles.begin();
  while (ptr != projectiles.end()) {
    if ((*ptr)->collidedWith(d)) {  
      freeList.push_back(*ptr);
      ptr = projectiles.erase(ptr);
      return true;
    }
    else ++ptr;
  }
  return false;

}

void ProjectilePool::reset()
{
  std::list<Projectile*>::const_iterator ptrProjectiles = projectiles.begin();
  while (ptrProjectiles != projectiles.end()) {
    delete (*ptrProjectiles);
    ++ptrProjectiles;
  }
  std::list<Projectile*>::const_iterator ptrFreeList = freeList.begin();
  while (ptrFreeList != freeList.end()) {
    delete (*ptrFreeList);
    ++ptrFreeList;
  }
  
  projectiles.clear();
  freeList.clear();
}