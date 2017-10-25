#ifndef PROJECTILE__H
#define PROJECTILE__H

#include <iostream>
#include "sprite.h"
#include "gamedata.h"
#include "collisionStrategy.h"

class Projectile : public Sprite {
public:

  explicit Projectile(
    const Vector2f& , const Vector2f ,
    const string& , const Frame* );

  Projectile(const Projectile& p);

  virtual void update(Uint32 ticks);
  bool goneTooFar() const;
  void reset();
  bool collidedWith(const Drawable* d) const;
  ~Projectile();
 
private:
  float time;
  bool tooFar;
  CollisionStrategy * strategy;
  
  Projectile& operator=(const Projectile&); // (Item 6)
};
#endif
