#ifndef MATCHCLIENTIDENTIFICATIONSTATE_H
#define MATCHCLIENTIDENTIFICATIONSTATE_H

#include <QtCore/QObject>

class MatchClientIdentificationState : public QObject
{
    Q_OBJECT
public:
    explicit MatchClientIdentificationState(QObject *parent = 0);

    Q_INVOKABLE void setCookie(const QString &cookie);

signals:
    void nextState();
    void cookieChanged(QString);
};

#endif // MATCHCLIENTIDENTIFICATIONSTATE_H
