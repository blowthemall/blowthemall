#include "matchservermatchstate.h"

MatchServerMatchState::MatchServerMatchState(QObject *parent) : QObject(parent)
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
