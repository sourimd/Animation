#include <SDL.h>
#include "ioManager.h"
#include "clock.h"
#include "sprite.h"
#include <vector>

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;

  SDL_Surface* footballSurface;
  const Frame * const footballFrame;
  std::vector<Sprite> footballs;

  int numberOfFootballs;

  SDL_Surface* backgroundSurface;
  const Frame * const backgroundFrame;
  Sprite background;

  bool makeVideo;
  int frameCount;
  std::string username;
  int frameMax;
  const std::string TITLE;

  void draw() const;
  void update();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void drawBackground() const;
};
