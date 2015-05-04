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

#include "global.h"
#include "blowthemallcontroller.h"
#include "matchserver.h"
#include "matchclient.h"
#include "gamescene.h"

#include <QtCore/QVariant>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>

BlowThemAllController::BlowThemAllController(QObject *parent)
    : QObject(parent)
    , gameScene(new GameScene{this})
    , gameView(new QGraphicsView{gameScene})
    , matchServer(new MatchServer{this})
    , matchClient(new MatchClient{gameScene, this})
{}

BlowThemAllController::~BlowThemAllController()
{
    delete gameView;
}

void BlowThemAllController::setComponentObject(QObject *component)
{
    component_ = component;
}

void BlowThemAllController::serveGame(quint16 port)
{
    emit matchStarted();
    auto l = new QLabel{"Starting..."};
    l->show();
    matchServer->listen(port);
    matchClient->connectToHost("127.0.0.1", port);
    connect(matchClient, &MatchClient::connected, [this,l]() {
        l->deleteLater();
        auto d = new QMessageBox;
        d->setText("Ready to play?");
        d->setButtonText(0, "Yes");
        d->setModal(false);
        d->show();
        connect(d, &QMessageBox::finished, [this,d]() {
            d->deleteLater();
            matchClient->notifyReadyToPlay();
            gameView->show();
        });
    });
}

void BlowThemAllController::joinGame(const QString &host, quint16 port)
{
    emit matchStarted();
    auto l = new QLabel{"Connecting..."};
    l->show();
    matchClient->connectToHost(host, port);
    connect(matchClient, &MatchClient::connected, [this,l]() {
        l->deleteLater();
        QMessageBox::information(l, "Ready?", "Ready to play?", "Yes");
        matchClient->notifyReadyToPlay();
        gameView->show();
    });
}

