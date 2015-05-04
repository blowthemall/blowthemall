#include "matchclientmatchsetupstate.h"

MatchClientMatchSetupState::MatchClientMatchSetupState(QObject *parent)
    : QObject(parent)
{}

void MatchClientMatchSetupState::matchStarted()
{
    emit nextState();
}

