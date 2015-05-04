#include "matchservermatchsetupstate.h"

MatchServerMatchSetupState::MatchServerMatchSetupState(QObject *parent)
    : QObject(parent)
{}

void MatchServerMatchSetupState::confirm()
{
    emit ready();
}

bool MatchServerMatchSetupState::abort()
{
    return false;
}

