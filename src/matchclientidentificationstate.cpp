#include "matchclientidentificationstate.h"

MatchClientIdentificationState::MatchClientIdentificationState(QObject *parent)
    : QObject(parent)
{}

void MatchClientIdentificationState::setCookie(const QString &cookie)
{
    emit cookieChanged(cookie);
    emit nextState();
}

