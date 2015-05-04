#ifndef MATCHCLIENTMATCHSTATE_H
#define MATCHCLIENTMATCHSTATE_H

#include <QtCore/QObject>

class MatchClientMatchState : public QObject
{
    Q_OBJECT
public:
    explicit MatchClientMatchState(QObject *parent = 0);

    Q_INVOKABLE void itemAdded(const QString &id, double x, double y, double z,
                               const QString &animationSet,
                               const QString &animationState);
    Q_INVOKABLE void itemAnimated(const QString &id,
                                  const QString &animationState);
    Q_INVOKABLE void itemMoved(const QString &id, double newX, double newY,
                               double newZ, double secs);
    Q_INVOKABLE void winner(const QString &id);

signals:
    void itemAddedSignal(QString id, double x, double y, double z,
                         QString animationSet, QString animationState);
    void itemAnimatedSignal(QString id, QString animationState);
    void itemMovedSignal(QString id, double newX, double newY, double newZ,
                         double secs);
    void winnerSignal(QString id);
};

#endif // MATCHCLIENTMATCHSTATE_H
