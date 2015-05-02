/*
  Copyright © 2012, 2015  Vinícius dos Santos Oliveira

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

#ifndef RPCNODE_H
#define RPCNODE_H

#include <QtCore/QObject>
#include <QtCore/QJsonValue>

#include <Tufao/AbstractMessageSocket>

#include <functional>

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(QObject *parent = 0);
    explicit Node(Tufao::AbstractMessageSocket *socket, QObject *parent = 0);
    ~Node();

    QObject *methods();
    void setMethods(QObject *object);

    Tufao::AbstractMessageSocket *messageSocket();
    void setMessageSocket(Tufao::AbstractMessageSocket *socket);

signals:
    void error(QJsonValue id, QByteArray data);

public slots:
    QJsonValue callWith(const QString &remoteMethod, const QJsonArray &args,
                        std::function<void(QJsonValue)> handler);
    QJsonValue callWith(const QString &remoteMethod, const QJsonObject &args,
                      std::function<void(QJsonValue)> handler);

    void call(const QString &remoteMethod, const QJsonArray &args);
    void call(const QString &remoteMethod, const QJsonObject &args);

private slots:
    void handleMessage(const QByteArray &msg);

private:
    enum Error
    {
        NO_ERROR,
        PARSE_ERROR      = -32700,
        INVALID_REQUEST  = -32600,
        METHOD_NOT_FOUND = -32601,
        INVALID_PARAMS   = -32602,
        INTERNAL_ERROR   = -32603
    };

    void handleRequest(const QJsonDocument &object);
    void handleResponse(const QJsonObject &object);

    QJsonObject processRequest(const QJsonObject &request);
    QPair<QJsonValue, QJsonObject>
    processReply(const QString &method, const QJsonValue &args);

    struct Priv;
    Priv *priv;
};

#endif // RPCNODE_H
