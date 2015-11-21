#ifndef BTA_GAME_MAP_H
#define BTA_GAME_MAP_H

#include <vector>
#include <cstdlib>

namespace bta
{

template<class T>
class GameMap
{
public:
  GameMap(size_t width, size_t height)
    : width_(width)
    , height_(height)
    , data(width * height)
  {}

  std::size_t width() const
  {
    return width_;
  }

  std::size_t height() const
  {
    return height_;
  }

private:
  size_t width_;
  size_t height_;
  std::vector<T> data;
};

}

#endif
