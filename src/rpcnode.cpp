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

#include "priv/rpcnode.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

using namespace Tufao;

Node::Node(QObject *parent) :
    QObject(parent),
    priv(new Priv)
{}

Node::Node(AbstractMessageSocket *socket, QObject *parent) :
    QObject(parent),
    priv(new Priv(socket))
{
    if (priv->socket) {
        connect(priv->socket, &AbstractMessageSocket::newMessage,
                this, &Node::handleMessage);
    }
}

Node::~Node()
{
    delete priv;
}

QObject *Node::methods()
{
    return priv->methods;
}

void Node::setMethods(QObject *object)
{
    priv->methods = object;
}

AbstractMessageSocket *Node::messageSocket()
{
    return priv->socket;
}

void Node::setMessageSocket(AbstractMessageSocket *socket)
{
    if (priv->socket) {
        disconnect(priv->socket, &AbstractMessageSocket::newMessage,
                   this, &Node::handleMessage);
    }

    priv->socket = socket;

    if (priv->socket) {
        connect(priv->socket, &AbstractMessageSocket::newMessage,
                this, &Node::handleMessage);
    }
}

QJsonValue Node::callWith(const QString &remoteMethod, const QJsonArray &args,
                          std::function<void(QJsonValue)> handler)
{
    return priv->callWith(remoteMethod, args, handler);
}

QJsonValue Node::callWith(const QString &remoteMethod, const QJsonObject &args,
                          std::function<void(QJsonValue)> handler)
{
    return priv->callWith(remoteMethod, args, handler);
}

void Node::call(const QString &remoteMethod, const QJsonArray &args)
{
    priv->call(remoteMethod, args);
}

void Node::call(const QString &remoteMethod, const QJsonObject &args)
{
    priv->call(remoteMethod, args);
}

void Node::handleMessage(const QByteArray &msg)
{
    QJsonParseError err;
    auto object = QJsonDocument::fromJson(msg, &err);

    if (err.error != QJsonParseError::NoError) {
        static const char data[] = "{\"jsonrpc\": \"2.0\", \"error\": "
                                   "{\"code\": -32700,"
                                   " \"message\": \"Parse error\"}, "
                                   "\"id\": null}";
        auto invalidJson = QByteArray::fromRawData(data, sizeof(data) - 1);
        priv->socket->sendMessage(invalidJson);
        return;
    }

    if (isRequestMessage(object)) {
        handleRequest(object);
    } else if (isResponseMessage(object)) {
        handleResponse(object.object());
    } else {
        static QByteArray invalidJsonRpc = "{\"jsonrpc\": \"2.0\", \"error\": "
                "{\"code\": -32600, \"message\": \"Invalid JSON-RPC.\"}, "
                "\"id\": null}";
        priv->socket->sendMessage(invalidJsonRpc);
    }
}

inline void Node::handleRequest(const QJsonDocument &object)
{
    if (object.isObject()) {
        QJsonObject response = processRequest(object.object());
        if (!response.isEmpty()) {
            priv->socket->sendMessage(QJsonDocument{response}
                                      .toJson(QJsonDocument::Compact));
        }
    } else if (object.isArray()) {
        QJsonArray batch = object.array();
        if (batch.isEmpty()) {
            priv->socket->sendMessage("{\"jsonrpc\": \"2.0\", \"error\": "
                                      "{\"code\": -32600, "
                                      "\"message\": \"Invalid JSON-RPC.\"}, "
                                      "\"id\": null}");
            return;
        }

        QJsonArray response;
        for (const QJsonValue &i: batch) {
            QJsonObject singleResponse = processRequest(i.toObject());
            if (!singleResponse.isEmpty())
                response.push_back(singleResponse);
        }
        priv->socket->sendMessage(QJsonDocument{response}
                                  .toJson(QJsonDocument::Compact));
    } else {
        priv->socket->sendMessage("{\"jsonrpc\": \"2.0\", \"error\": "
                                  "{\"code\": -32600, "
                                  "\"message\": \"Invalid JSON-RPC.\"}, "
                                  "\"id\": null}");
    }
}

inline void Node::handleResponse(const QJsonObject &object)
{
    if (!object.contains("id"))
        return;

    if (object.contains("error")) {
        QJsonValue error = object["error"];
        if (!error.isObject())
            return;

        emit this->error(object["id"],
                         QJsonDocument{error.toObject()}
                         .toJson(QJsonDocument::Compact));
        return;
    }

    QJsonValue key = object["id"];
    if (!object.contains("result") || !priv->calls.contains(key))
        return;

    priv->calls[key](object["result"]);
    priv->calls.remove(key);
}

QJsonObject Node::processRequest(const QJsonObject &request)
{
    QJsonObject reply({{QString("jsonrpc"), QJsonValue("2.0")}});

    if (!request.contains("method")) {
        reply["error"] = QJsonObject{{{"code", -32600},
                                      {"message", "Invalid Request."}}};
        reply["id"] = QJsonValue{};
        return reply;
    }

    QJsonValue method = request["method"];

    if (!method.isString()) {
                reply["error"] = QJsonObject{{{"code", -32600},
                                             {"message", "Invalid Request."}}};
                reply["id"] = QJsonValue{};
                return reply;
    }

    if (request.contains("params")) {
        QJsonValue params = request["params"];
        if (!params.isArray() && !params.isObject() && !params.isNull()) {
                    reply["error"] = QJsonObject{{
                        {"code", -32600},
                        {"message", "Invalid Request."}
                    }};
                    reply["id"] = QJsonValue{};
                    return reply;
        }
    }

    if (request.contains("id")) {
        QJsonValue id = request["id"];
        if (!id.isString() && !id.isDouble() && !id.isNull()) {
            reply["error"] = QJsonObject{{{"code", -32600},
                                         {"message", "Invalid Request."}}};
            reply["id"] = QJsonValue{};
            return reply;
        }
        reply["id"] = id;
    }

    QPair<QJsonValue, QJsonObject> result
        = processReply(method.toString(), request.value("params"));
    if (!reply.contains("id"))
        return {};

    if (!result.second.isEmpty()) {
        reply["error"] = result.second;
        return reply;
    }

    reply["result"] = result.first;
    return reply;
}

inline QPair<QJsonValue, QJsonObject> Node::processReply(const QString &method,
                                                         const QJsonValue &args)
{
    if (!priv->methods
            || (!args.isArray() && !args.isUndefined())) {
        return qMakePair(QJsonValue{},
                         QJsonObject{{{"code", METHOD_NOT_FOUND},
                                      {"message", "Method not found"}}});
    }

    QJsonArray params = args.toArray();

    // 10 is the max number of args supported by moc/qobject/...
    if (params.size() > 10) {
        return qMakePair(QJsonValue{},
                         QJsonObject{{{"code", METHOD_NOT_FOUND},
                                      {"message", "Method not found"}}});
    }

    const QMetaObject *metaObject = priv->methods->metaObject();
    int i = indexOfMethod(metaObject, method, params);

    if (i == -1) {
        return qMakePair(QJsonValue{},
                         QJsonObject{{{"code", METHOD_NOT_FOUND},
                                      {"message", "Method not found"}}});
    }

    QMetaMethod metaMethod = metaObject->method(i);

    {
        int size = params.size();
        QVector<ArgumentHandler> argsHandler;
        QVector<QGenericArgument> args;

        argsHandler.resize(size);
        args.reserve(size);

        for (int i = 0;i != size;++i)
            args.push_back(argsHandler[i].convert(params[i]));

        ReturnHandler retHandler;
        QGenericReturnArgument ret = retHandler.convert(metaMethod.typeName());

        metaMethod.invoke(priv->methods, Qt::DirectConnection, ret,
                          (0 < size) ? args[0] : QGenericArgument(),
                          (1 < size) ? args[1] : QGenericArgument(),
                          (2 < size) ? args[2] : QGenericArgument(),
                          (3 < size) ? args[3] : QGenericArgument(),
                          (4 < size) ? args[4] : QGenericArgument(),
                          (5 < size) ? args[5] : QGenericArgument(),
                          (6 < size) ? args[6] : QGenericArgument(),
                          (7 < size) ? args[7] : QGenericArgument(),
                          (8 < size) ? args[8] : QGenericArgument(),
                          (9 < size) ? args[9] : QGenericArgument());

        return qMakePair(QJsonValue{retHandler}, QJsonObject{});
    }
}
