#include "mapmodel.h"
#include "global.h"
#include <stdexcept>

MapModel::MapModel()
    : data(MAP_WIDTH * MAP_HEIGHT)
{}

int MapModel::width()
{
    return MAP_WIDTH;
}

int MapModel::height()
{
    return MAP_HEIGHT;
}

MapTile &MapModel::element(int x, int y)
{
    if (x < 0)
        throw std::out_of_range{"x cannot be less than 0"};
    if (y < 0)
        throw std::out_of_range{"y cannot be less than 0"};
    if (x >= MAP_WIDTH)
        throw std::out_of_range{"x cannot be greater or equals than MAP_WIDTH"};
    if (y >= MAP_HEIGHT) {
        throw std::out_of_range{"x cannot be greater or equals than"
                                " MAP_HEIGHT"};
    }

    return data[y * MAP_WIDTH + x];
}

void MapModel::generateMap()
{
    for (int i = 0; i != MAP_WIDTH; ++i) {
        for (int j = 0;j != MAP_HEIGHT;++j) {
            element(i, j).level
                    = (i % 2 && j % 2) ? MapTile::Level::UNBREAKABLE
                                       : MapTile::Level::BREAKABLE;
            element(i, j).items.clear();
        }
    }
    element(0, 0).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(0, 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(1, 0).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 1, 0).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 2, 0).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 1, 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(0, height() - 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(0, height() - 2).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(1, height() - 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 1, height() - 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 2, height() - 1).level = MapTile::Level::NON_BLOCKING_GROUND;
    element(width() - 1, height() - 2).level = MapTile::Level::NON_BLOCKING_GROUND;
}

