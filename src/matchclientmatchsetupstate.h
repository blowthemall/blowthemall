#ifndef MATCHCLIENTMATCHSETUPSTATE_H
#define MATCHCLIENTMATCHSETUPSTATE_H

#include <QtCore/QObject>

class MatchClientMatchSetupState : public QObject
{
    Q_OBJECT
public:
    explicit MatchClientMatchSetupState(QObject *parent = 0);

    Q_INVOKABLE void matchStarted();

signals:
    void nextState();
};

#endif // MATCHCLIENTMATCHSETUPSTATE_H
