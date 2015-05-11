#include "gamescene.h"
#include "global.h"

#include <QtGui/QKeyEvent>
#include <QtCore/QTimer>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, parent)
    , keys{
           -1,
           -1,
           -1,
           -1,
           -1
       }
    , timer(new QTimer{this})
{
    timer->setInterval(KEY_EVENT_MAX_LATENCY);
    timer->start();
    connect(timer, &QTimer::timeout, this, &GameScene::onTimer);
}

void GameScene::keyPressEvent(QKeyEvent *keyEvent)
{
    int idx = keyIndex(keyEvent);
    if (idx == -1)
        return;

    keys[idx] = timer->remainingTime();
}

void GameScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    int idx = keyIndex(keyEvent);
    if (idx == -1)
        return;

    double value = keys[idx] + (KEY_EVENT_MAX_LATENCY - timer->remainingTime());
    keys[idx] = -1;
    switch (idx) {
    case Keys::UP:
        emit moveUp(double{value} / 1000.);
        break;
    case Keys::RIGHT:
        emit moveRight(double{value} / 1000.);
        break;
    case Keys::DOWN:
        emit moveDown(double{value} / 1000.);
        break;
    case Keys::LEFT:
        emit moveLeft(double{value} / 1000.);
        break;
    case Keys::DROP_BOMB:
        emit dropBomb();
    }
}

void GameScene::onTimer()
{
    if (keys[Keys::UP] > 0)
        emit moveUp(double{KEY_EVENT_MAX_LATENCY} / 1000.);
    if (keys[Keys::RIGHT] > 0)
        emit moveRight(double{KEY_EVENT_MAX_LATENCY} / 1000.);
    if (keys[Keys::DOWN] > 0)
        emit moveDown(double{KEY_EVENT_MAX_LATENCY} / 1000.);
    if (keys[Keys::LEFT] > 0)
        emit moveLeft(double{KEY_EVENT_MAX_LATENCY} / 1000.);
}

int GameScene::keyIndex(QKeyEvent *keyEvent)
{
    switch (keyEvent->key()) {
    case Qt::Key_Up:
        return Keys::UP;
    case Qt::Key_Right:
        return Keys::RIGHT;
    case Qt::Key_Down:
        return Keys::DOWN;
    case Qt::Key_Left:
        return Keys::LEFT;
    case Qt::Key_Space:
        return Keys::DROP_BOMB;
    default:
        return -1;
    }
}
