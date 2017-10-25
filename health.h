#include "vector2f.h"
#include "ioManager.h"
#include "aaline.h"

class Health {
public:
  static Health& getInstance();
  ~Health() {};
  void draw(SDL_Surface*) const;
  void update(Uint32);
  void reset() { currentLength = totalLength; }
private:
  Vector2f start;
  int totalLength;
  int currentLength;
  int thick;
  int increments;
  float interval;
  int deltaTime;

  void drawBox(SDL_Surface*) const;
  Health(const Health&);
  Health& operator=(const Health&);
  Health();
  Health(int sx, int sy, int tl, int cl,
    float t, int inc, float sp);
};
