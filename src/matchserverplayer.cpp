#include "matchserverplayer.h"
#include "matchserverbroadcast.h"
#include "matchservermatchsetupstate.h"
#include "matchservermatchstate.h"
#include "rpcnode.h"
#include <Tufao/WebSocket>
#include <QtCore/QJsonArray>

using namespace Tufao;

MatchServerPlayer::MatchServerPlayer(WebSocket *ws,
                                     MatchServerBroadcast *broadcast)
    : QObject(ws)
    , states{
          new MatchServerMatchSetupState{this},
          new MatchServerMatchState{this}
      }
    , node(new RpcNode{ws, this})
{
    node->setMethods(states[0]);
    node->call("setCookie", QJsonArray{{id}});

    connect(static_cast<MatchServerMatchSetupState*>(states[0]),
            &MatchServerMatchSetupState::ready, [this,broadcast]() {
        node->setMethods(NULL);
        broadcast->notifyClientReady(id);
    });
    connect(static_cast<MatchServerMatchState*>(states[1]),
            &MatchServerMatchState::moveUpSignal,
            [this,broadcast](double seconds) {
        broadcast->notifyMoveUp(id, seconds);
    });
    connect(static_cast<MatchServerMatchState*>(states[1]),
            &MatchServerMatchState::moveRightSignal,
            [this,broadcast](double seconds) {
        broadcast->notifyMoveRight(id, seconds);
    });
    connect(static_cast<MatchServerMatchState*>(states[1]),
            &MatchServerMatchState::moveDownSignal,
            [this,broadcast](double seconds) {
        broadcast->notifyMoveDown(id, seconds);
    });
    connect(static_cast<MatchServerMatchState*>(states[1]),
            &MatchServerMatchState::moveLeftSignal,
            [this,broadcast](double seconds) {
        broadcast->notifyMoveLeft(id, seconds);
    });
    connect(static_cast<MatchServerMatchState*>(states[1]),
            &MatchServerMatchState::dropBombSignal, [this,broadcast]() {
        broadcast->notifyDropBomb(id);
    });
    connect(broadcast, &MatchServerBroadcast::matchStarted, [this]() {
        node->setMethods(states[1]);
        node->call("matchStarted", QJsonArray{});
    });
    connect(broadcast, &MatchServerBroadcast::itemAdded,
            [this](const QString &id, double x, double y, double z,
                   const QString &animationSet, const QString &animationState) {
        node->call("itemAdded", QJsonArray{{id, x, y, z, animationSet,
                                            animationState}});
    });
    connect(broadcast, &MatchServerBroadcast::itemAnimated,
            [this](const QString &id, const QString &animationState) {
        node->call("itemAnimated", QJsonArray{{id, animationState}});
    });
    connect(broadcast, &MatchServerBroadcast::itemMoved,
            [this](const QString &id, double newX, double newY, double newZ,
                   double secs) {
        node->call("itemMoved", QJsonArray{{id, newX, newY, newZ, secs}});
    });
    connect(broadcast, &MatchServerBroadcast::winner,
            [this](const QString &id) {
        node->call("winner", QJsonArray{{id}});
    });
}
