#ifndef BTA_GAME_MAP_H
#define BTA_GAME_MAP_H

#include <vector>
#include <pair>
#include <cstdlib>

namespace bta
{

enum class Axis { X, Y };

Axis complement(Axis axis)
{
  switch (axis) {
  case Axis::X:
    return Axis::Y;
  case Axis::Y:
    return Axis::X;
  }
}

struct SubtilePosRaw1D
{
  unsigned filled;
  unsigned increasing;
};

SubtilePosRaw1D makeSubtilePosRaw1D(unsigned filled, unsigned increasing)
{
  return SubtilePosRaw1D{filled, increasing};
}

struct MovementResult
{
  unsigned tileOffset;
  SubtilePosRaw1D newSubtilePos;
};

MovementResult move(SubtilePosRaw1D currentPos, int steps,
                    unsigned granularity);

template<class T>
struct Tile
{
  enum class Terrain
  {
    UNBREAKABLE,
    BREAKABLE,
    NON_BLOCKING
  };

  class MoveableItem
  {
  public:
    std::pair<int, int> subtilePos() const;

    void center(Axis axis)
    {
      switch (axis) {
      case Axis::X:
        x_filled = FILLED_MAX;
        break;
      case Axis::Y:
        y_filled = FILLED_MAX;
        break;
      }
    }

    /**
       \p offset is given in subtile units (the step unit). Returns
       the offset represented as tile units.

       Whenever you call this function, you shall:

       1. Use the returned value to compute which tile will own this
          `MoveableItem` now.
       2. Check adjacent tiles and `subtilePos()` to detect if this
          item is occupying an unreachable tile. Call `center(axis)` if
          necessary.
       3. Check if there were blocks in adjacent tiles along the way
          that would force the `MoveableItem` to also move in the
          other axis. If this is the case, call
          `center(complement(axis))` and penalize speed of this
          `MoveableItem` a little..
     */
    unsigned move(int offset, Axis axis)
    {
      switch (axis) {
      case Axis::X:
        auto res = move(makeSubtilePosRaw1D(x_filled, !going_left), offset,
                        GRANULARITY);
        going_left = !res.newSubtilePos.increasing;
        x_filled = res.newSubtilePos.filled;
        return res.tileOffset;
      case Axis::Y:
        auto res = move(makeSubtilePosRaw1D(y_filled, !going_up), offset,
                        GRANULARITY);
        going_up = !res.newSubtilePos.increasing;
        y_filled = res.newSubtilePos.filled;
        return res.tileOffset;
      }
    }

    T item;

  private:
    static constexpr unsigned FILLED_MIN = 0;
    static constexpr unsigned FILLED_MAX = 7;

    /* (filled_max() - filled_min()) possible states at each side and the
       centered state */
    static constexpr unsigned GRANULARITY = 2 * (FILLED_MAX - FILLED_MIN) + 1;

    // TODO: do some magic to make the following use as much space as `T`
    unsigned going_up: 1;
    unsigned y_filled: 3;
    unsigned going_left: 1;
    unsigned x_filled: 3;
  };

  Terrain terrain;
  std::vector<T> items;
}

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
  std::vector<Tile<T>> data;
};

}

#endif
