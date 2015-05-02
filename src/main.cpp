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

#include <memory>

#include <QtGui/QIcon>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlComponent>
#include <QtWidgets/QApplication>

#include "blowthemallcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    BlowThemAllController controller;

    app.setApplicationDisplayName("BlowThemAll");
    app.setWindowIcon(QIcon(":/gfx/logo.png"));

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("blowthemall", &controller);

    QQmlComponent component(&engine, QUrl(QStringLiteral("qrc:///ui/main.qml")));
    auto object = std::unique_ptr<QObject>(component.create());
    controller.setComponentObject(object.get());
    object->findChild<QObject*>("versionText")->setProperty("text", 'v' + QString{BLOWTHEMALL_VERSION});

    return app.exec();
}
