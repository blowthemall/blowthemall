/*
  Copyright © 2015  Vinícius dos Santos Oliveira

  This file is part of BlowThemAll.

  BlowThemAll is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */

#include "matchserver.h"
#include <algorithm>
#include <QtCore/QJsonArray>
#include <Tufao/HttpServer>
#include <Tufao/HttpServerResponse>
#include <Tufao/WebSocket>
#include "matchserverbroadcast.h"
#include "matchserverplayer.h"

using namespace Tufao;

MatchServer::MatchServer(QObject *parent)
    : QObject(parent)
    , server(new HttpServer(this))
    , broadcast(new MatchServerBroadcast{this})
{
    connect(this, &MatchServer::matchStarted,
            broadcast, &MatchServerBroadcast::matchStarted);
    connect(broadcast, &MatchServerBroadcast::clientReady,
            [this](const QString &id) {
        players.push_back(id);

        if (++ready < connected)
            return;

        emit matchStarted();
        startMatchImpl();
    });
    connect(broadcast, &MatchServerBroadcast::moveUp,
            this, &MatchServer::onMoveUp);
    connect(broadcast, &MatchServerBroadcast::moveRight,
            this, &MatchServer::onMoveRight);
    connect(broadcast, &MatchServerBroadcast::moveDown,
            this, &MatchServer::onMoveDown);
    connect(broadcast, &MatchServerBroadcast::moveLeft,
            this, &MatchServer::onMoveLeft);
    connect(broadcast, &MatchServerBroadcast::dropBomb,
            this, &MatchServer::onDropBomb);

    auto onRequestReady = [](HttpServerRequest&, HttpServerResponse &res) {
        res.writeHead(HttpResponseStatus::NOT_FOUND);
        res.end("BlowThemAll match server\n");
    };
    connect(server, &HttpServer::requestReady, onRequestReady);

    auto upgradeHandler = [this](HttpServerRequest &req,
                                 const QByteArray &head) {
        auto ws = new WebSocket{this};
        if (!ws->startServerHandshake(req, head)) {
            ws->deleteLater();
            return;
        }

        ws->setMessagesType(WebSocketMessageType::TEXT_MESSAGE);
        connect(ws, &AbstractMessageSocket::disconnected,
                ws, &QObject::deleteLater);

        new MatchServerPlayer(ws, broadcast);
        ++connected;
    };
    server->setUpgradeHandler(upgradeHandler);
}

bool MatchServer::listen(quint16 port)
{
    return server->listen(QHostAddress::Any, port);
}

QString MatchServer::getNick(const QString &id)
{
    return id;
}

void MatchServer::startMatch()
{
}

void MatchServer::stopMatch()
{
}

void MatchServer::onMoveUp(const QString &id, double secs)
{
    auto &idx = map.index()[id];
    auto &cur_tile = map.element(idx.first, idx.second);
    auto avatar_it = find_if(cur_tile.items.begin(), cur_tile.items.end(),
                             [&id](const MapMoveableElement &v) {
        return v.id == id;
    });

    unsigned displacement = avatar_it->moveUp(secs);
    if (displacement && idx.second != 0) {
        if (displacement > idx.second) {
            idx.second = 0;
            avatar_it->y_filled = MapMoveableElement::filled_max();
        } else {
            idx.second -= displacement;
        }

        map.element(idx.first, idx.second).items.push_back(*avatar_it);
        cur_tile.items.erase(avatar_it);
        avatar_it = map.element(idx.first, idx.second).items.end() - 1;
    }

    if (idx.second == 0 && (displacement || avatar_it->is_up == true))
        avatar_it->y_filled = MapMoveableElement::filled_max();

    broadcast->moveItem(id, idx.first + avatar_it->xWithinTile(),
                        idx.second + avatar_it->yWithinTile(), 0, secs / 2.);
}

void MatchServer::onMoveRight(const QString &id, double secs)
{
    auto &idx = map.index()[id];
    auto &cur_tile = map.element(idx.first, idx.second);
    auto avatar_it = find_if(cur_tile.items.begin(), cur_tile.items.end(),
                             [&id](const MapMoveableElement &v) {
        return v.id == id;
    });

    unsigned displacement = avatar_it->moveRight(secs);
    if (displacement && idx.first != map.width() - 1) {
        idx.first += displacement;
        if (idx.first >= map.width()) {
            idx.first = map.width() - 1;
            avatar_it->x_filled = MapMoveableElement::filled_max();
        }

        map.element(idx.first, idx.second).items.push_back(*avatar_it);
        cur_tile.items.erase(avatar_it);
        avatar_it = map.element(idx.first, idx.second).items.end() - 1;
    }

    if (idx.first == map.width() - 1
        && (displacement || avatar_it->is_left == false)) {
        avatar_it->x_filled = MapMoveableElement::filled_max();
    }

    broadcast->moveItem(id, idx.first + avatar_it->xWithinTile(),
                        idx.second + avatar_it->yWithinTile(), 0, secs / 2.);
}

void MatchServer::onMoveDown(const QString &id, double secs)
{
    auto &idx = map.index()[id];
    auto &cur_tile = map.element(idx.first, idx.second);
    auto avatar_it = find_if(cur_tile.items.begin(), cur_tile.items.end(),
                             [&id](const MapMoveableElement &v) {
        return v.id == id;
    });

    unsigned displacement = avatar_it->moveDown(secs);
    if (displacement && idx.second != map.height() - 1) {
        idx.second += displacement;
        if (idx.second >= map.height()) {
            idx.second = map.height() - 1;
            avatar_it->y_filled = MapMoveableElement::filled_max();
        }

        map.element(idx.first, idx.second).items.push_back(*avatar_it);
        cur_tile.items.erase(avatar_it);
        avatar_it = map.element(idx.first, idx.second).items.end() - 1;
    }

    if (idx.second == map.height() - 1
        && (displacement || avatar_it->is_up == false)) {
        avatar_it->y_filled = MapMoveableElement::filled_max();
    }

    broadcast->moveItem(id, idx.first + avatar_it->xWithinTile(),
                        idx.second + avatar_it->yWithinTile(), 0, secs / 2.);
}

void MatchServer::onMoveLeft(const QString &id, double secs)
{
    auto &idx = map.index()[id];
    auto &cur_tile = map.element(idx.first, idx.second);
    auto avatar_it = find_if(cur_tile.items.begin(), cur_tile.items.end(),
                             [&id](const MapMoveableElement &v) {
        return v.id == id;
    });

    unsigned displacement = avatar_it->moveLeft(secs);
    if (displacement && idx.first != 0) {
        if (displacement > idx.first) {
            idx.first = 0;
            avatar_it->x_filled = MapMoveableElement::filled_max();
        } else {
            idx.first -= displacement;
        }

        map.element(idx.first, idx.second).items.push_back(*avatar_it);
        cur_tile.items.erase(avatar_it);
        avatar_it = map.element(idx.first, idx.second).items.end() - 1;
    }

    if (idx.first == 0 && (displacement || avatar_it->is_left == true))
        avatar_it->x_filled = MapMoveableElement::filled_max();

    broadcast->moveItem(id, idx.first + avatar_it->xWithinTile(),
                        idx.second + avatar_it->yWithinTile(), 0, secs / 2.);
}

void MatchServer::onDropBomb(const QString &id)
{

}

void MatchServer::startMatchImpl()
{
    map.generateMap();
    for (int i = 0;i != map.width();++i) {
        for (int j = 0;j != map.height();++j) {
            auto id = "b" + QString::number(i) + "," + QString::number(j);
            switch (map.element(i, j).level) {
            case MapTile::Level::BREAKABLE:
                broadcast->addItem(id, i, j, 1, "basic", "first");
                break;
            case MapTile::Level::UNBREAKABLE:
                broadcast->addItem(id, i, j, 1, "eternal", "first");
            case MapTile::Level::NON_BLOCKING_GROUND:
                ;
            }
        }
    }
    {
        MapMoveableElement e;
        e.kind = MapMoveableElement::ElementKind::AVATAR;
        e.x_filled = 7;
        e.y_filled = 7;
        e.id = players.front();
        map.element(0, 0).items.push_back(e);
        map.index()[e.id] = QPair<int, int>(0, 0);
        broadcast->addItem(e.id, 0, 0, 0, "simplechar", "standing-down");
    }
    if (players.size() > 1) {
        MapMoveableElement e;
        e.kind = MapMoveableElement::ElementKind::AVATAR;
        e.x_filled = 7;
        e.y_filled = 7;
        e.id = players[1];
        map.element(map.width() - 1, 0).items.push_back(e);
        map.index()[e.id] = QPair<int, int>(map.width() - 1, 0);
        broadcast->addItem(e.id, map.width() - 1, 0, 0, "simplechar",
                           "standing-down");
    }
    if (players.size() > 2) {
        MapMoveableElement e;
        e.kind = MapMoveableElement::ElementKind::AVATAR;
        e.x_filled = 7;
        e.y_filled = 7;
        e.id = players[2];
        map.element(0, map.height() - 1).items.push_back(e);
        map.index()[e.id] = QPair<int, int>(0, map.height() - 1);
        broadcast->addItem(e.id, 0, map.height() - 1, 0, "simplechar",
                           "standing-down");
    }
    if (players.size() > 3) {
        MapMoveableElement e;
        e.kind = MapMoveableElement::ElementKind::AVATAR;
        e.x_filled = 7;
        e.y_filled = 7;
        e.id = players[3];
        map.element(map.width() - 1, map.height() - 1).items.push_back(e);
        map.index()[e.id] = QPair<int, int>(map.width() - 1, map.height() - 1);
        broadcast->addItem(e.id, map.width() - 1, map.height() - 1, 0,
                           "simplechar", "standing-down");
    }
}
