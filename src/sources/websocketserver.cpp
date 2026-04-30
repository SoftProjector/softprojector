/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2023  Vladislav Kobzar
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

#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"

#include "../headers/websocketserver.hpp"

#include <QJsonObject>

WebSocketServer::WebSocketServer() { }

void WebSocketServer::startServer(QString httpServerIPAddress, quint16 port) {
    server = new QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, this);
    if (server->listen(QHostAddress(httpServerIPAddress), port)) {
        qDebug() << "Websocket server listening on " << httpServerIPAddress << ":" << port;
        connect(server, &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(server, &QWebSocketServer::closed, this, &WebSocketServer::closed);
        isRunning = true;
    }
}

void WebSocketServer::onNewConnection()
{
    QWebSocket *pSocket = server->nextPendingConnection();

    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    clients << pSocket;

    sendToClients();
}

void WebSocketServer::sendToClients()
{
    QString output = QJsonDocument(jsonOutput).toJson(QJsonDocument::Compact);
    for (int i = 0; i < clients.size(); i++) {
        clients[i]->sendTextMessage(output);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void WebSocketServer::closingClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void WebSocketServer::stopServer()
{
    server->close();
    qDeleteAll(clients.begin(), clients.end());
    isRunning = false;
}

void WebSocketServer::setBibleText(Verse verse)
{
    jsonOutput = QJsonObject();

    QJsonObject bible;

    QJsonObject primary;
    primary["verse"] = verse.primary_text;
    primary["reference"] = (verse.primary_caption == ":" ? "" : verse.primary_caption);
    bible["primary"] = primary;

    QJsonObject secondary;
    secondary["verse"] = verse.secondary_text;
    secondary["reference"] =(verse.secondary_caption == ":" ? "" : verse.secondary_caption);
    bible["secondary"] = secondary;

    QJsonObject trinary;
    trinary["verse"] = verse.trinary_text;
    trinary["reference"] = (verse.trinary_caption == ":" ? "" : verse.trinary_caption);
    bible["trinary"] = trinary;

    jsonOutput["bible"] = bible;

    sendToClients();
}

void WebSocketServer::setSongText(QString verse, QString verseTranslation, QString splitVerse, QString splitVerseTranslation, bool isLast)
{
    jsonOutput = QJsonObject();

    QJsonObject jsonSong;

    jsonSong["verse"] = verse;
    jsonSong["verseTranslation"] = verseTranslation;
    jsonSong["split"] = splitVerse;
    jsonSong["splitTranslation"] = splitVerseTranslation;
    jsonSong["isLast"] = isLast;

    jsonOutput["song"] = jsonSong;

    sendToClients();
}

void WebSocketServer::setBlank()
{
    jsonOutput = QJsonObject();

    sendToClients();
}
