#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include "drawable.h"

class TwoWayMultiSprite : public Drawable {
public:
  TwoWayMultiSprite(const std::string&, const std::string&);

  virtual ~TwoWayMultiSprite() { }

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const; 
  void setDirectionLeft() {
      isFacingRight = false;
      isFacingLeft = true;
  }
  void setDirectionRight() {
    isFacingRight = true;
    isFacingLeft = false;
  }

  bool getRight() { return isFacingRight; }
protected:
  const std::vector<Frame *> frames;
  const std::vector<Frame *> reverseFrames;
  int worldWidth;
  int worldHeight;
  bool isFacingRight;
  bool isFacingLeft;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
  TwoWayMultiSprite(const TwoWayMultiSprite&);
  TwoWayMultiSprite& operator=(const TwoWayMultiSprite&);
};
#endif
