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

#ifndef INTROSPECTION_H
#define INTROSPECTION_H

#include <QtCore/QMetaMethod>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

QByteArray typeName(const QJsonValue &value)
{
    switch (value.type()) {
    case QJsonValue::Array:
        return "QJsonArray";
    case QJsonValue::Bool:
        return "bool";
    case QJsonValue::Double:
        return "double";
    case QJsonValue::Object:
        return "QJsonObject";
    case QJsonValue::String:
        return "QString";
    case QJsonValue::Null:
    case QJsonValue::Undefined:
    default:
        return {};
    }
}

inline int indexOfMethod(const QMetaObject *metaObject,
                         const QString &methodName,
                         const QJsonArray &params,
                         bool lookForAllTypes = false)
{
    for (int i = 0;i < metaObject->methodCount();++i) {
        QMetaMethod m = metaObject->method(i);

        if (!lookForAllTypes
                && m.methodType() != QMetaMethod::Method)
            continue;

        if (methodName != m.name())
            continue;

        QList<QByteArray> parameterTypes = m.parameterTypes();

        if (params.size() != parameterTypes.size())
            continue;

        bool paramsOk = true;
        for (int j = 0;j < params.size();++j) {
            if (typeName(params[j]) != parameterTypes[j]) {
                paramsOk = false;
                break;
            }
        }

        if (paramsOk)
            return i;
    }

    return -1;
}

class ArgumentHandler
{
public:
    QGenericArgument convert(const QJsonValue &variant)
    {
        const char *name;
        const void *data;

        switch (variant.type()) {
        case QJsonValue::String:
            name = "QString";
            stringBuffer = variant.toString();
            data = &stringBuffer;
            break;
        case QJsonValue::Double:
            name = "double";
            doubleBuffer = variant.toDouble();
            data = &doubleBuffer;
            break;
        case QJsonValue::Object:
            name = "QJsonObject";
            mapBuffer = variant.toObject();
            data = &mapBuffer;
            break;
        case QJsonValue::Array:
            name = "QJsonArray";
            listBuffer = variant.toArray();
            data = &listBuffer;
            break;
        case QJsonValue::Bool:
            name = "bool";
            boolBuffer = variant.toBool();
            data = &boolBuffer;
            break;
        default:
            // This should never happen, because all possible JSON values
            // have been mapped in the previous code
            return QGenericArgument();
        }

        return QGenericArgument(name, data);
    }

private:
    QString stringBuffer;
    double doubleBuffer;
    QJsonObject mapBuffer;
    QJsonArray listBuffer;
    bool boolBuffer;
};

class ReturnHandler
{
public:
    QGenericReturnArgument convert(const QByteArray &typeName)
    {
        QGenericReturnArgument ret;

        if (typeName == "QString") {
            ret = Q_RETURN_ARG(QString, stringBuffer);
            used = QJsonValue::String;
        } else if (typeName == "double") {
            ret = Q_RETURN_ARG(double, doubleBuffer);
            used = QJsonValue::Double;
        } else if (typeName == "QJsonObject") {
            ret = Q_RETURN_ARG(QJsonObject, mapBuffer);
            used = QJsonValue::Object;
        } else if (typeName == "QJsonArray") {
            ret = Q_RETURN_ARG(QJsonArray, listBuffer);
            used = QJsonValue::Array;
        } else if (typeName == "bool") {
            ret = Q_RETURN_ARG(bool, boolBuffer);
            used = QJsonValue::Bool;
        }

        return ret;
    }

    operator QJsonValue ()
    {
        switch (used) {
        case QJsonValue::String:
            return stringBuffer;
        case QJsonValue::Double:
            return doubleBuffer;
        case QJsonValue::Object:
            return mapBuffer;
        case QJsonValue::Array:
            return listBuffer;
        case QJsonValue::Bool:
            return boolBuffer;
        default:
            return QJsonValue{QJsonValue::Undefined};
        }
    }

private:
    QString stringBuffer;
    double doubleBuffer;
    QJsonObject mapBuffer;
    QJsonArray listBuffer;
    bool boolBuffer;

    QJsonValue::Type used;
};

#endif // INTROSPECTION_H
