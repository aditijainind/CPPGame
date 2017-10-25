#include <cmath>
#include "SmartEnemy.h"
#include "gamedata.h"
#include "player.h"
SmartEnemy::SmartEnemy(const std::string& name) :
  Drawable(name,
    Vector2f(rand() % Gamedata::getInstance().getXmlInt(name + "/maximumX") + Gamedata::getInstance().getXmlInt(name + "/minStartLocX"),
      rand() % Gamedata::getInstance().getXmlInt(name + "/maximumY") + Gamedata::getInstance().getXmlInt(name + "/minimumY")),
    Vector2f(rand() % Gamedata::getInstance().getXmlInt(name + "/maxSpeedX") + Gamedata::getInstance().getXmlInt(name + "/minSpeedX"),
      0)
    ),
  explosion(NULL),
  frame(FrameFactory::getInstance().getFrame(name)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  attackDistanceX(Gamedata::getInstance().getXmlInt(name + "/attackDistanceX")),
  attackVelocityY(Gamedata::getInstance().getXmlInt(name + "/speedY"))
{ }

SmartEnemy::SmartEnemy(const string& n, const Vector2f& pos, const Vector2f& vel) :
  Drawable(n, pos, vel),
  explosion(NULL),
  frame(FrameFactory::getInstance().getFrame(n)),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  attackDistanceX(Gamedata::getInstance().getXmlInt(n + "/attackDistanceX")),
  attackVelocityY(Gamedata::getInstance().getXmlInt(n + "/speedY"))
{ }

SmartEnemy::SmartEnemy(const string& n, const Vector2f& pos, const Vector2f& vel,
  const Frame* frm) :
  Drawable(n, pos, vel),
  explosion(NULL),
  frame(frm),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  attackDistanceX(Gamedata::getInstance().getXmlInt(n + "/attackDistanceX")),
  attackVelocityY(Gamedata::getInstance().getXmlInt(n + "/speedY"))
{ }

SmartEnemy::SmartEnemy(const SmartEnemy& s) :
  Drawable(s),
  explosion(s.explosion),
  frame(s.frame),
  frameWidth(s.getFrame()->getWidth()),
  frameHeight(s.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  attackDistanceX(s.attackDistanceX),
  attackVelocityY(s.attackVelocityY)
{ }

SmartEnemy& SmartEnemy::operator=(const SmartEnemy& rhs) {
  Drawable::operator=(rhs);
  explosion = rhs.explosion;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

void SmartEnemy::draw() const {
  if (explosion) {
    explosion->draw();
    return;
  }
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y);
}

void SmartEnemy::explode() {
  if (explosion) return;
  explosion = new ExplodingSprite(this);
}

void SmartEnemy::update(Uint32 ticks) {
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if (Y() < 0) {
    velocityY(abs(velocityY()));
  }
  if (Y() > worldHeight - frameHeight) {
    velocityY(-abs(velocityY()));
  }

  if (X() < 0) {
    velocityX(abs(velocityX()));
  }
  if (X() > worldWidth - frameWidth) {
    velocityX(-abs(velocityX()));
  }

  if (abs((Player::getInstance()).X() - X()) < attackDistanceX)
  {
    if (Y() > (Player::getInstance()).Y())
      velocityY(-abs(attackVelocityY));
    else if (Y() < (Player::getInstance()).Y())
      velocityY(abs(attackVelocityY));
  }

}

int SmartEnemy::getDistance(const SmartEnemy *obj) const {
  return hypot(X() - obj->X(), Y() - obj->Y());
}
