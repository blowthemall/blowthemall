#ifndef BTAITEM_H
#define BTAITEM_H

#include <QtWidgets/QGraphicsObject>

class BtaItem : public QGraphicsObject
{
public:
    BtaItem(const QString &animationSet, const QString &animationState);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

public slots:
    void loadState(const QString &state);

private:
    QString animationSet;
    QString animationState;
};

#endif // BTAITEM_H
