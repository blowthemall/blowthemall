#ifndef MATCHSERVERBROADCAST_H
#define MATCHSERVERBROADCAST_H

#include <QtCore/QObject>

class MatchServerBroadcast : public QObject
{
    Q_OBJECT
public:
    explicit MatchServerBroadcast(QObject *parent = 0);

    // Functions called by **SERVER**
    void startMatch();
    void stopMatch();
    void addItem(const QString &id, double x, double y, double z,
                 const QString &animationSet, const QString &animationState);
    void animateItem(const QString &id, const QString &animationState);
    void moveItem(const QString &id, double newX, double newY, double newZ,
                  double secs);
    void notifyWinner(const QString &id);

    // Functions called by **CLIENT**
    void notifyClientReady(const QString &id);
    void notifyMoveUp(const QString &id, double seconds);
    void notifyMoveRight(const QString &id, double seconds);
    void notifyMoveDown(const QString &id, double seconds);
    void notifyMoveLeft(const QString &id, double seconds);
    void notifyDropBomb(const QString &id);

signals:
    // Notifications watched by **CLIENT**
    void matchStarted();
    void matchStopped();
    void itemAdded(QString id, double x, double y, double z,
                   QString animationSet, QString animationState);
    void itemAnimated(QString id, QString animationState);
    void itemMoved(QString id, double newX, double newY, double newZ,
                   double secs);
    void winner(QString id);

    // Notifications watched by **SERVER**
    void clientReady(QString id);
    void moveUp(QString id, double seconds);
    void moveRight(QString id, double seconds);
    void moveDown(QString id, double seconds);
    void moveLeft(QString id, double seconds);
    void dropBomb(QString id);
};

#endif // MATCHSERVERBROADCAST_H
