#ifndef BLOWTHENALLGAMELOOP_H
#define BLOWTHENALLGAMELOOP_H

#include <SDL_events.h>
#include <SDL_render.h>

namespace bta
{

class GameLoop
{
public:
  GameLoop();
  virtual ~GameLoop();

  void run();

protected:
  virtual void unload();
  virtual void events(SDL_Event *event);
  virtual void update(Uint32 gameTime);
  virtual void draw() const;

  SDL_Renderer *render;
  SDL_Window *window;
  bool gameExit;
};

}
#endif
