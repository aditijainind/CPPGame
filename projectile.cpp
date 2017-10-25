#include <iostream>
#include <cmath>

#include "projectile.h"

Projectile:: Projectile(
  const Vector2f& pos, const Vector2f vel,
  const string& name, const Frame* fm) :
                Sprite(name, pos, vel, fm),
                time(0),

                tooFar(false),
                strategy(new MidPointCollisionStrategy())
{
  velocityY(0);
}

Projectile::Projectile(const Projectile& p) : Sprite(p),
                time(p.time),
                tooFar(p.tooFar),
                strategy(new PerPixelCollisionStrategy())
{

}

void Projectile::update(Uint32 ticks) {
  float xincr = velocityX() * static_cast<float>(ticks) * 0.001;
  X(X() - xincr);
  time+=ticks;
  if (time > 1500) 
  {
   tooFar = true;
   time = 0;
  }
}

bool Projectile::goneTooFar() const
{
  return tooFar; 
}

void Projectile::reset() {
  tooFar = false;
  time = 0;
}

bool Projectile::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
}

Projectile::~Projectile()
{
  delete strategy;
}
