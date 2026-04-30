/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2022  Vladislav Kobzar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef WEBSOCKETSERVER_HPP
#define WEBSOCKETSERVER_HPP

#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>

#include "bible.hpp"
#include "song.hpp"
#include "announcement.hpp"

// WebSocket implementation taken from https://code.qt.io/cgit/qt/qtwebsockets.git/tree/examples/websockets?h=5.15

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class WebSocketServer : public QObject
{
    Q_OBJECT

public:
    WebSocketServer();
    void startServer(QString httpServerIPAddress, quint16 port);
    void stopServer();
    void setBibleText(Verse verse);
    void setSongText(QString verse, QString verseTranslation = "", QString splitVerse = "", QString splitVerseTranslation = "", bool isLast = false);
    void setBlank();
    bool isRunning;

public slots:
    void closingClient();

private:
    QWebSocketServer *server;
    QList<QWebSocket *> clients;
    qint64 bytesAvailable() const;
    QJsonObject jsonOutput;
    void establishConnection();
    bool servingConnection;
    void sendToClients();
    void onNewConnection();
    void socketDisconnected();

signals:
    void closed();
};

#endif
