#ifndef MAPMOVEABLEELEMENT_H
#define MAPMOVEABLEELEMENT_H

#include <QtCore/QString>

struct MapMoveableElement
{
    enum class ElementKind
    {
        AVATAR,
        BOMB
    };

    unsigned is_up: 1;
    unsigned y_filled: 3; // [0,8)
    unsigned is_left: 1;
    unsigned x_filled: 3; // [0,8)

    QString id;
    ElementKind kind;
};

#endif // MAPMOVEABLEELEMENT_H
