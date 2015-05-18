#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QtCore/QMap>
#include "maptile.h"

class MapModel
{
public:
    MapModel();

    int width();
    int height();
    MapTile& element(int x, int y);

    QMap<QString, QPair<int, int>> &index()
    {
        // This function is sooo shitty
        /* It should be replaced by:

           - MapTile access is wrapped by a guard object.
           - index synchronization is handled by the guard destructor (RAII) */
        return index_;
    }

    void generateMap();

private:
    std::vector<MapTile> data;
    QMap<QString, QPair<int, int>> index_;
};

#endif // MAPMODEL_H
