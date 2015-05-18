#include "matchservermatchstate.h"

using namespace std;

MatchServerMatchState::MatchServerMatchState(QObject *parent)
    : QObject(parent)
    , lastMoveCommand(chrono::steady_clock::now())
{}

void MatchServerMatchState::moveUp(double seconds)
{
    emit moveUpSignal(seconds);
}

void MatchServerMatchState::moveRight(double seconds)
{
    emit moveRightSignal(seconds);
}

void MatchServerMatchState::moveDown(double seconds)
{
    if (!isMovementValid(seconds))
        return;

    emit moveDownSignal(seconds);
}

void MatchServerMatchState::moveLeft(double seconds)
{
    emit moveLeftSignal(seconds);
}

void MatchServerMatchState::dropBomb()
{
    emit dropBombSignal();
}

inline bool MatchServerMatchState::isMovementValid(double seconds)
{
    auto now = chrono::steady_clock::now();
    return now > lastMoveCommand + chrono::duration<decltype(seconds)>{seconds};
}
