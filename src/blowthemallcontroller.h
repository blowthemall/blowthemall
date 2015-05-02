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

#ifndef BLOWTHEMALLCONTROLLER_H
#define BLOWTHEMALLCONTROLLER_H

#include <QtCore/QObject>

class QGraphicsView;

class BlowThemAllController : public QObject
{
    Q_OBJECT
public:
    explicit BlowThemAllController(QObject *parent = 0);
    ~BlowThemAllController();

    void setComponentObject(QObject *component);

signals:
    void matchStarted();
    void matchStopped();

public slots:
    void serveGame(quint16 port);
    void joinGame(const QString &host, quint16 port);

private:
    QObject *component_ = NULL;
    QGraphicsView *gameView;
};

#endif // BLOWTHEMALLCONTROLLER_H
