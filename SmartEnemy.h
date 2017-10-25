#ifndef SMARTENEMY__H
#define SMARTENEMY__H
#include <string>
#include "drawable.h"

class ExplodingSprite;
class SmartEnemy : public Drawable {
public:
  SmartEnemy(const std::string&);
  SmartEnemy(const std::string&, const Vector2f& pos, const Vector2f& vel);
  SmartEnemy(const std::string&,
    const Vector2f& pos, const Vector2f& vel, const Frame*);
  SmartEnemy(const SmartEnemy& s);
  virtual ~SmartEnemy() { }
  SmartEnemy& operator=(const SmartEnemy&);

  virtual const Frame* getFrame() const { return frame; }
  virtual void draw() const;

  virtual void update(Uint32 ticks);
  void explode();

private:
  ExplodingSprite* explosion;
  const Frame * frame;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
  int getDistance(const SmartEnemy*) const;
  float attackDistanceX;
  float attackVelocityY;
};
#endif
