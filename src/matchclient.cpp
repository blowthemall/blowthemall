#include "matchclient.h"
#include "matchclientidentificationstate.h"
#include "matchclientmatchsetupstate.h"
#include "matchclientmatchstate.h"
#include "rpcnode.h"
#include "global.h"
#include "gamescene.h"
#include "btaitem.h"

#include <Tufao/WebSocket>

#include <QtCore/QJsonArray>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QMessageBox>

using namespace Tufao;

MatchClient::MatchClient(GameScene *scene, QObject *parent)
    : QObject(parent)
    , scene(scene)
    , node(new RpcNode{this})
    , states{
          new MatchClientIdentificationState{this},
          new MatchClientMatchSetupState{this},
          new MatchClientMatchState{this}
      }
{
    connect(static_cast<MatchClientIdentificationState*>(states[0]),
            &MatchClientIdentificationState::cookieChanged,
            [this](const QString &newCookie) {
        cookie = newCookie;
    });
    connect(static_cast<MatchClientIdentificationState*>(states[0]),
            &MatchClientIdentificationState::nextState, [this]() {
        node->setMethods(states[1]);
    });
    connect(static_cast<MatchClientMatchSetupState*>(states[1]),
            &MatchClientMatchSetupState::nextState, [this]() {
        node->setMethods(states[2]);
    });
    connect(static_cast<MatchClientMatchState*>(states[2]),
            &MatchClientMatchState::itemAddedSignal,
            [this](const QString &id, double x, double y, double z,
                   const QString &animationSet, const QString &animationState) {
        auto item = new BtaItem{animationSet, animationState};
        items[id] = item;
        item->setPos(x * TILE_SIZE, y * TILE_SIZE);
        item->setZValue(z);
        this->scene->addItem(item);
    });
    connect(static_cast<MatchClientMatchState*>(states[2]),
            &MatchClientMatchState::itemAnimatedSignal,
            [this](const QString &id, const QString &animationState) {
        if (!items.contains(id)) {
            qDebug("itemAnimated: Unrecognized item id");
            return;
        }

        items[id]->loadState(animationState);
    });
    connect(static_cast<MatchClientMatchState*>(states[2]),
            &MatchClientMatchState::itemMovedSignal,
            [this](const QString &id, double newX, double newY, double newZ,
                   double secs) {
        if (!items.contains(id)) {
            qDebug("itemMoved: Unrecognized item id");
            return;
        }

        items[id]->setZValue(newZ);
        auto animation = new QPropertyAnimation(items[id], "pos", this);
        animation->setEndValue(QPointF{newX * TILE_SIZE, newY * TILE_SIZE});
        animation->setDuration(secs * 1000);
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });
    connect(static_cast<MatchClientMatchState*>(states[2]),
            &MatchClientMatchState::winnerSignal, [this](const QString &id) {
        auto we_won = id == cookie;
        QMessageBox::information(NULL, "Winner",
                                 "Winner: " + id + (we_won ? " (you)"
                                                           : " (not you)"));
        QCoreApplication::quit();
    });
    connect(scene, &GameScene::moveUp, [this](double secs) {
        if (node->methods() != states[2])
            return;

        node->call("moveUp", QJsonArray{{secs}});
    });
    connect(scene, &GameScene::moveRight, [this](double secs) {
        if (node->methods() != states[2])
            return;

        node->call("moveRight", QJsonArray{{secs}});
    });
    connect(scene, &GameScene::moveDown, [this](double secs) {
        if (node->methods() != states[2])
            return;

        node->call("moveDown", QJsonArray{{secs}});
    });
    connect(scene, &GameScene::moveLeft, [this](double secs) {
        if (node->methods() != states[2])
            return;

        node->call("moveLeft", QJsonArray{{secs}});
    });
    connect(scene, &GameScene::dropBomb, [this]() {
        if (node->methods() != states[2])
            return;

        node->call("dropBomb", QJsonArray{});
    });
}

void MatchClient::connectToHost(const QString &host, quint16 port)
{
    auto ws = new WebSocket{this};
    ws->connectToHost(host, port, "/");
    ws->setMessagesType(WebSocketMessageType::TEXT_MESSAGE);
    connect(ws, &AbstractMessageSocket::connected, [this,ws]() {
        node->setMethods(states[0]);
        node->setMessageSocket(ws);
        emit connected();
    });

    connect(ws, &AbstractMessageSocket::disconnected, [this,ws]() {
        node->setMessageSocket(NULL);
        ws->deleteLater();
    });
}

void MatchClient::notifyReadyToPlay()
{
    node->call("confirm", QJsonArray{});
}

