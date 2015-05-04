#ifndef MATCHSERVERPLAYER_H
#define MATCHSERVERPLAYER_H

#include <QtCore/QObject>
#include <QtCore/QUuid>

namespace Tufao {
class WebSocket;
}

class MatchServerBroadcast;
class RpcNode;

class MatchServerPlayer : public QObject
{
    Q_OBJECT
public:
    /**
     * @param ws It's also set as the parent of this object.
     */
    explicit MatchServerPlayer(Tufao::WebSocket *ws,
                               MatchServerBroadcast *broadcast);

private:
    QString id = QUuid::createUuid().toString();
    QObject *states[2];
    RpcNode *node;
    MatchServerBroadcast *broadcast;
};

#endif // MATCHSERVERPLAYER_H
