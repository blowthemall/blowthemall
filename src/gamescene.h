#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QtWidgets/QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = NULL);

signals:
    void moveUp(double seconds);
    void moveRight(double seconds);
    void moveDown(double seconds);
    void moveLeft(double seconds);
    void dropBomb();

protected:
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;

private slots:
    void onTimer();

private:
    enum Keys
    {
        UP,
        RIGHT,
        DOWN,
        LEFT,
        DROP_BOMB
    };

    static int keyIndex(QKeyEvent *keyEvent);

    int keys[5];
    QTimer *timer;
};

#endif // GAMESCENE_H
