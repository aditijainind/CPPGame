#include "player.h"
#include "gamedata.h"

Player::~Player() {}

Player::Player(const std::string& oneWaySpec, const std::string& otherWaySpec) :
  TwoWayMultiSprite(oneWaySpec, otherWaySpec),
  explosion(NULL),
   
  playerVelocity(getVelocity()),
  startPos(getPosition()),
  isStopped(true),
  isShooting(false),
  velocityXFromXml(abs(velocityX())),
  velocityYFromXml(abs(velocityY())),
  minimumX(Gamedata::getInstance().getXmlInt(oneWaySpec + "/minimumX")),
  minimumY(Gamedata::getInstance().getXmlInt(oneWaySpec + "/minimumY")),
  maximumX(Gamedata::getInstance().getXmlInt(oneWaySpec + "/maximumX")),
  maximumY(Gamedata::getInstance().getXmlInt(oneWaySpec + "/maximumY")),
  projectilePool(oneWaySpec),
  strategy(new RectangularCollisionStrategy),
  initialHealth(Gamedata::getInstance().getXmlInt(oneWaySpec + "/health")),
  playerHealth(Gamedata::getInstance().getXmlInt(oneWaySpec + "/health")),
  initialLives(Gamedata::getInstance().getXmlInt(oneWaySpec + "/lives")),
  playerLives(Gamedata::getInstance().getXmlInt(oneWaySpec + "/lives")),
  isGodMode()
{
}

Player& Player::getInstance()
{
  static Player player = Player("PlayerRight", "PlayerLeft");
  return player;
}
void Player::draw() const
{
  if (explosion) {
    explosion->draw();
    return;
  }

  if (isShooting)
  {
    projectilePool.draw();
  }
  TwoWayMultiSprite::draw();
}

void Player::explode() {
  if (explosion) return;
  explosion = new ExplodingSprite(this);
}

void Player::shoot() {
  isShooting = true;
  if(TwoWayMultiSprite::getRight())
    projectilePool.shoot(getPosition()+Vector2f(getFrame()->getWidth(),0), playerVelocity*2,getName());
  else
    projectilePool.shoot(getPosition(), -playerVelocity*2,getName());
}

void Player::update(Uint32 ticks)
{
  if (explosion) {
    explosion->update(ticks);
    if (explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  if (isShooting)
  {
    projectilePool.update(ticks);
    //isShooting = false;
  }

  if (!isStopped)
  {
    advanceFrame(ticks);
    float currentX = X();
    float currentY = Y();
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    if (currentX + incr[0] > maximumX)
      X(maximumX);
    else if (currentX + incr[0] < minimumX)
      X(minimumX);
    else
      X(currentX + incr[0]);
    
    if (currentY + incr[1] > maximumY)
      Y(maximumY);
    else if (currentY + incr[1] < minimumY)
      Y(minimumY);
    else
      Y(currentY + incr[1]);
  }
}
void Player::Stop()
{
  isStopped = true;
  velocityY(0);
  velocityX(0);
}
void Player::Up()
{
  isStopped = false;
  if (Y() < minimumY)
    velocityY(0);
  else
  velocityY(-abs(velocityYFromXml));
}
void Player::Down()
{
  isStopped = false;
  if (Y() > maximumY)
    velocityY(0);

  else
    velocityY(abs(velocityYFromXml));
}
void Player::Left()
{
  setDirectionLeft();
  isStopped = false;
  if (X() < 0)
    velocityX(0);
  else
    velocityX(-abs(velocityXFromXml));
}
void Player::Right()
{
  setDirectionRight();
  isStopped = false;
  if (X() > maximumX)
    velocityX(0);
  else
    velocityX(abs(velocityXFromXml));
}

void Player::reset()
{ 
  setPosition(startPos); 
  setDirectionRight();
  projectilePool.reset();
  isGodMode = false;
  resetHealth();
  resetLives();
}

bool Player::collidedWith(const Drawable* d) const {
  return strategy->execute(*this, *d);
}

bool Player::checkForBulletCollisions(const Drawable* d)
{
  return projectilePool.checkForBulletCollisions(d);
}

void Player::setGodMode()
{
  isGodMode = !isGodMode;
}
bool Player::getGodMode()
{
  return isGodMode;
}

void Player::reduceHealth()
{
  playerHealth--;
}

int Player::getHealth() 
{
  return playerHealth;
}

void Player::resetHealth()
{
  playerHealth = initialHealth;
}

void Player::reduceLives()
{
  playerLives--;
}

int Player::getLives()
{
  return playerLives;
}

void Player::resetLives()
{
  playerLives = initialLives;
}