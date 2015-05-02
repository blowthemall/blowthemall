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

#ifndef PRIV_RPCNODE_H
#define PRIV_RPCNODE_H

#include "../rpcnode.h"
#include "introspection.h"

#include <QtCore/QVector>
#include <QtCore/QJsonDocument>

inline bool operator<(const QJsonValue &lhs, const QJsonValue &rhs)
{
    if (lhs.type() != rhs.type())
        return lhs.type() < rhs.type();

    switch (lhs.type()) {
    case QJsonValue::Array:
        return lhs.toArray() < rhs.toArray();
    case QJsonValue::Bool:
        return lhs.toBool() < rhs.toBool();
    case QJsonValue::Double:
        return lhs.toDouble() < rhs.toDouble();
    case QJsonValue::Null:
    case QJsonValue::Undefined:
        return false;
    case QJsonValue::Object:
        return lhs.toObject() < rhs.toObject();
    case QJsonValue::String:
        return lhs.toString() < rhs.toString();
    }
    return false;
}

inline bool isRequestMessage(const QJsonDocument &object)
{
    return object.isArray()
            || (object.isObject() && object.object().contains("method"));
}

inline bool isResponseMessage(const QJsonDocument &doc)
{
    // Because libbta doesn't create batch requests, batch responses aren't
    // received.
    if (!doc.isObject())
        return false;

    auto obj = doc.object();
    return obj.contains("result") || obj.contains("error");
}

template<class T>
QJsonObject jsonRpcObject(const QString &remoteMethod, const T &args)
{
    QJsonObject object;

    object["jsonrpc"] = "2.0";
    object["method"] = remoteMethod;

    if (!args.isEmpty())
        object["params"] = args;

    return object;
}

struct Node::Priv
{
    Priv(Tufao::AbstractMessageSocket *socket = NULL) :
        socket(socket)
    {}

    template<class T>
    QJsonValue callWith(const QString &remoteMethod, const T &args,
                      std::function<void (QJsonValue)> handler)
    {
        if (!socket)
            return {QJsonValue::Undefined};

        QJsonValue id = index++;
        QJsonObject object = jsonRpcObject(remoteMethod, args);
        object["id"] = id;

        if (!socket->sendMessage(QJsonDocument{object}
                                 .toJson(QJsonDocument::Compact))) {
            return {QJsonValue::Undefined};
        }

        calls[id] = handler;
        return id;
    }

    template<class T>
    void call(const QString &remoteMethod, const T &args)
    {
        if (!socket)
            return;

        socket->sendMessage(QJsonDocument{jsonRpcObject(remoteMethod, args)}
                            .toJson(QJsonDocument::Compact));
    }

    QObject *methods = NULL;
    Tufao::AbstractMessageSocket *socket = NULL;
    double index = 0;

    QMap<QJsonValue, std::function<void (QJsonValue)>> calls;
};

#endif // PRIV_RPCNODE_H
