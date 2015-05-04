#ifndef MAPTILE_H
#define MAPTILE_H

#include <cstdint>
#include <vector>
#include "mapmoveableelement.h"

struct MapTile
{
    enum class Level: std::uint8_t
    {
        UNBREAKABLE,
        BREAKABLE,
        NON_BLOCKING_GROUND
    };

    Level level;
    std::vector<MapMoveableElement> items;
};

#endif // MAPTILE_H
