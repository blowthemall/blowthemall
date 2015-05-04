#ifndef MATCHSERVERMATCHSETUPSTATE_H
#define MATCHSERVERMATCHSETUPSTATE_H

#include <QtCore/QObject>

class MatchServerMatchSetupState : public QObject
{
    Q_OBJECT
public:
    explicit MatchServerMatchSetupState(QObject *parent = 0);

    Q_INVOKABLE void confirm();
    Q_INVOKABLE bool abort();

signals:
    void ready();
};

#endif // MATCHSERVERMATCHSETUPSTATE_H
