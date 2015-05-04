#include "btaitem.h"
#include "global.h"
#include <QtGui/QPainter>

BtaItem::BtaItem(const QString &animationSet, const QString &animationState)
    : animationSet(animationSet)
    , animationState(animationState)
{}

QRectF BtaItem::boundingRect() const
{
    return QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE};
}

void BtaItem::paint(QPainter *painter,
                    const QStyleOptionGraphicsItem */*option*/,
                    QWidget */*widget*/)
{
    if (animationSet == "basic") {
        if (animationState == "first") {
            painter->drawImage(QRectF{0, 0, TILE_SIZE, TILE_SIZE},
                               QImage{":/gfx/basic/b0.svg"});
        } else if (animationState == "exploding") {
            painter->drawImage(QRectF{0, 0, TILE_SIZE, TILE_SIZE},
                               QImage{":/gfx/basic/b1.svg"});
        } else if (animationState == "exploded") {
            ;
        } else {
            qDebug("Unrecognized animation state");
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::Dense3Pattern);
            painter->drawRect(0, 0, TILE_SIZE, TILE_SIZE);
        }
    } else if (animationSet == "simplechar") {
        if (animationState == "walking-up") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/costas 1.svg"});
        } else if (animationState == "walking-right") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/D1.svg"});
        } else if (animationState == "walking-down") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/F1.svg"});
        } else if (animationState == "walking-left") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/E3.svg"});
        } else if (animationState == "standing-up") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/costas 2.svg"});
        } else if (animationState == "standing-right") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/D2.svg"});
        } else if (animationState == "standing-down") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/F2.svg"});
        } else if (animationState == "standing-left") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/E2.svg"});
        } else if (animationState == "dying") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/b3.svg"});
        } else if (animationState == "dead") {
            ;
        } else if (animationState == "winning") {
            painter->drawImage(QRectF{0, -TILE_SIZE, TILE_SIZE, 2 * TILE_SIZE},
                               QImage{":/gfx/simplechar/w2.svg"});
        } else {
            qDebug("Unrecognized animation state");
            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::Dense3Pattern);
            painter->drawRect(0, 0, TILE_SIZE, TILE_SIZE);
        }
    } else {
        qDebug("Unrecognized animation set");
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::Dense3Pattern);
        painter->drawRect(0, 0, TILE_SIZE, TILE_SIZE);
    }
}

void BtaItem::loadState(const QString &state)
{
    animationState = state;
    update();
}

