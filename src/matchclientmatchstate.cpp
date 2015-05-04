#include "matchclientmatchstate.h"

MatchClientMatchState::MatchClientMatchState(QObject *parent) : QObject(parent)
{}

void MatchClientMatchState::itemAdded(const QString &id, double x, double y,
                                      double z, const QString &animationSet,
                                      const QString &animationState)
{
    emit itemAddedSignal(id, x, y, z, animationSet, animationState);
}

void MatchClientMatchState::itemAnimated(const QString &id,
                                         const QString &animationState)
{
    emit itemAnimatedSignal(id, animationState);
}

void MatchClientMatchState::itemMoved(const QString &id, double newX,
                                      double newY, double newZ, double secs)
{
    emit itemMovedSignal(id, newX, newY, newZ, secs);
}

void MatchClientMatchState::winner(const QString &id)
{
    emit winnerSignal(id);
}

