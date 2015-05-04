#include "matchserverbroadcast.h"

MatchServerBroadcast::MatchServerBroadcast(QObject *parent) : QObject(parent)
{}

void MatchServerBroadcast::startMatch()
{
    emit matchStarted();
}

void MatchServerBroadcast::stopMatch()
{
    emit matchStopped();
}

void MatchServerBroadcast::addItem(const QString &id, double x, double y,
                                   double z, const QString &animationSet,
                                   const QString &animationState)
{
    emit itemAdded(id, x, y, z, animationSet, animationState);
}

void MatchServerBroadcast::animateItem(const QString &id,
                                       const QString &animationState)
{
    emit itemAnimated(id, animationState);
}

void MatchServerBroadcast::moveItem(const QString &id, double newX, double newY,
                                    double newZ, double secs)
{
    emit itemMoved(id, newX, newY, newZ, secs);
}

void MatchServerBroadcast::notifyWinner(const QString &id)
{
    emit winner(id);
}

void MatchServerBroadcast::notifyClientReady(const QString &id)
{
    emit clientReady(id);
}

void MatchServerBroadcast::notifyMoveUp(const QString &id, double seconds)
{
    emit moveUp(id, seconds);
}

void MatchServerBroadcast::notifyMoveRight(const QString &id, double seconds)
{
    emit moveRight(id, seconds);
}

void MatchServerBroadcast::notifyMoveDown(const QString &id, double seconds)
{
    emit moveDown(id, seconds);
}

void MatchServerBroadcast::notifyMoveLeft(const QString &id, double seconds)
{
    emit moveLeft(id, seconds);
}

void MatchServerBroadcast::notifyDropBomb(const QString &id)
{
    emit dropBomb(id);
}
