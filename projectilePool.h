#include <vector>
#include <list>
#include "projectile.h"
#include "frameFactory.h"
class ProjectilePool  {
public:
  ProjectilePool(const string&);
  ~ProjectilePool();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  unsigned int projectileCount() const { return projectiles.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
  virtual const Frame* getFrame() const { return projectileFrame; }
  void shoot(const Vector2f &, const Vector2f &, const string&);
  bool checkForBulletCollisions(const Drawable* );
  void reset();

private:
  const Frame * projectileFrame;
  float frameInterval; 
  float ticksSinceLastFrame;
 
  std::list<Projectile*> projectiles;
  std::list<Projectile*> freeList; 
  ProjectilePool(const ProjectilePool&); 
  ProjectilePool& operator=(const ProjectilePool&);
};
