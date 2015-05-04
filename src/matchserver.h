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

#ifndef MATCHSERVER_H
#define MATCHSERVER_H

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include "mapmodel.h"

namespace Tufao {
class HttpServer;
}

class MatchServerBroadcast;

class MatchServer : public QObject
{
    Q_OBJECT
public:
    explicit MatchServer(QObject *parent = 0);

    bool listen(quint16 port);

    Q_INVOKABLE QString getNick(const QString &id);

signals:
    void playerConnected(QString id);
    void playerDisconnected(QString id);
    void matchStarted();

public slots:
    void startMatch();
    void stopMatch();

private slots:
    void onMoveUp(const QString &id, double secs);
    void onMoveRight(const QString &id, double secs);
    void onMoveDown(const QString &id, double secs);
    void onMoveLeft(const QString &id, double secs);
    void onDropBomb(const QString &id);

private:
    void startMatchImpl();

    Tufao::HttpServer *server;
    MatchServerBroadcast *broadcast;
    int connected = 0;
    int ready = 0;

    // Game logic
    struct Bomb
    {
        QString id;
        qint64 msecsRemaining;

        bool operator<(const Bomb &o)
        {
            return id < o.id;
        }
    };

    MapModel map;
    QStringList players;
    QMap<QString, quint64> bombs;
};

#endif // MATCHSERVER_H
