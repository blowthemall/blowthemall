#include "gameloop.h"

#include <SDL.h>

#include <stdexcept>

#include "global.h"

using namespace bta;

GameLoop::GameLoop() : gameExit(false)
{
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
     throw std::runtime_error("could not initialize everything from sdl2");

  window = nullptr;
  render = nullptr;

  window = SDL_CreateWindow(BLOWTHENALL_WINDOW_TITLE,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, 800, 600,
                            SDL_WINDOW_ALLOW_HIGHDPI);

  render = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC |
                              SDL_RENDERER_ACCELERATED);

  if(window == nullptr)
    throw std::runtime_error("could not create a window");
  if(render == nullptr)
    throw std::runtime_error("could not create renderer");
}

GameLoop::~GameLoop()
{
  unload();
}

void
GameLoop::run()
{
  SDL_Event event;
  while(not gameExit)
  {
    while(SDL_PollEvent(&event))
      events(&event);

    update(SDL_GetTicks());

    draw();
  }
}

void
GameLoop::draw() const
{
  SDL_RenderClear(render);

  SDL_RenderPresent(render);
}

void
GameLoop::events(SDL_Event *event)
{
  if(event->type == SDL_QUIT)
   gameExit = true;
  if(event->type == SDL_KEYUP and event->key.keysym.sym == SDLK_ESCAPE)
   gameExit = true;
}

void
GameLoop::unload()
{
  SDL_DestroyRenderer(render);
  SDL_DestroyWindow(window);
}

void
GameLoop::update(Uint32 gameTime)
{
}
