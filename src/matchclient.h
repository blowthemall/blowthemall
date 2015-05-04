#ifndef MATCHCLIENT_H
#define MATCHCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QHash>

class GameScene;
class BtaItem;
class RpcNode;

class MatchClient : public QObject
{
    Q_OBJECT
public:
    explicit MatchClient(GameScene *scene, QObject *parent = 0);

    void connectToHost(const QString &host, quint16 port);

    void notifyReadyToPlay();

signals:
    void connected();

private:
    GameScene *scene;
    QHash<QString, BtaItem*> items;
    RpcNode *node;
    QObject *states[3];
    QString cookie;
};

#endif // MATCHCLIENT_H
