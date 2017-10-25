#include "twowaymultisprite.h"
#include "gamedata.h"
#include "frameFactory.h"

void TwoWayMultiSprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame + 1) % numberOfFrames;
    timeSinceLastFrame = 0;
  }
}

const Frame* TwoWayMultiSprite::getFrame() const {
  if (velocityX() < 0 || velocityY()<0)
    return reverseFrames[currentFrame];
  return frames[currentFrame];
}

TwoWayMultiSprite::TwoWayMultiSprite(const std::string& oneWaySpec, const std::string& otherWaySpec) :
  Drawable(oneWaySpec,
    Vector2f(Gamedata::getInstance().getXmlInt(oneWaySpec + "/startLoc/x"),
      Gamedata::getInstance().getXmlInt(oneWaySpec + "/startLoc/y")),
    Vector2f(Gamedata::getInstance().getXmlInt(oneWaySpec + "/speedX"),
      Gamedata::getInstance().getXmlInt(oneWaySpec + "/speedY"))
    ),
  frames(FrameFactory::getInstance().getFrames(oneWaySpec)),
  reverseFrames(FrameFactory::getInstance().getFrames(otherWaySpec)),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  isFacingRight(true),
  isFacingLeft(false),
  currentFrame(0),
  numberOfFrames(Gamedata::getInstance().getXmlInt(oneWaySpec + "/frames")),
  frameInterval(Gamedata::getInstance().getXmlInt(oneWaySpec + "/frameInterval")),
  timeSinceLastFrame(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }


void TwoWayMultiSprite::draw() const {
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());

  if (isFacingRight)
    frames[currentFrame]->draw(x, y);
  if(isFacingLeft)
    reverseFrames[currentFrame]->draw(x, y);
    
}

void TwoWayMultiSprite::update(Uint32 ticks) {
  advanceFrame(ticks);
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
}
