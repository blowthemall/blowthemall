#ifndef MAPMODEL_H
#define MAPMODEL_H

#include "maptile.h"

class MapModel
{
public:
    MapModel();

    int width();
    int height();
    MapTile& element(int x, int y);

    void generateMap();

private:
    std::vector<MapTile> data;
};

#endif // MAPMODEL_H
