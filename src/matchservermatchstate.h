#ifndef MATCHSERVERMATCHSTATE_H
#define MATCHSERVERMATCHSTATE_H

#include <QtCore/QObject>
#include <chrono>

class MatchServerMatchState : public QObject
{
    Q_OBJECT
public:
    explicit MatchServerMatchState(QObject *parent = 0);

    Q_INVOKABLE void moveUp(double seconds);
    Q_INVOKABLE void moveRight(double seconds);
    Q_INVOKABLE void moveDown(double seconds);
    Q_INVOKABLE void moveLeft(double seconds);
    Q_INVOKABLE void dropBomb();

signals:
    void moveUpSignal(double seconds);
    void moveRightSignal(double seconds);
    void moveDownSignal(double seconds);
    void moveLeftSignal(double seconds);
    void dropBombSignal();

private:
    bool isMovementValid(double seconds);

    std::chrono::steady_clock::time_point lastMoveCommand;
};

#endif // MATCHSERVERMATCHSTATE_H
