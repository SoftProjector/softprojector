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

#include "../headers/httpserver.hpp"

#include <QFile>
#include <QJsonObject>
#include <qtcpsocket.h>

HttpServer::HttpServer() { }

void HttpServer::startServer(QString httpServerIPAddress, quint16 httpServerPort, quint16 webSocketServerPort)
{
    ipAddress = httpServerIPAddress;
    httpPort = httpServerPort;
    webSocketPort = webSocketServerPort;
    server = new QTcpServer();

    // waiting for the web brower to make contact, this will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(startConnection()));

    if(server->listen(QHostAddress(ipAddress), httpPort)) {
        isRunning = true;
        qDebug() << "HTTP server listening on " << ipAddress << ":" << httpPort;
    } else {
        qDebug() << "HTTP server failed listening on " << ipAddress << ":" << httpPort;
    }
}

void HttpServer::startConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(sendData()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(closingClient()));
}

void HttpServer::closingClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void HttpServer::sendData()
{
    QTcpSocket* socket = (QTcpSocket*)sender();

    QFile file(":/html/html/httpserver.html");
    if(!file.open(QFile::ReadOnly |
                  QFile::Text))
    {
        qDebug() << "Could not open httpserver.html for reading";
        return;
    }

    QTextStream in(&file);
    QString html = in.readAll();
    file.close();

    html.replace("${ipAddress}", ipAddress);
    html.replace("${webSocketPort}", QString::number(webSocketPort));

    socket->write("HTTP/1.1 200 OK\r\n");       // \r needs to be before \n
    socket->write("Content-Type: text/html\r\n");
    socket->write("Connection: close\r\n");     // Require two \r\n.
    socket->write("\r\n");

    socket->write(html.toUtf8());

    socket->disconnectFromHost();
}

void HttpServer::stopServer()
{
    server->close();
    isRunning = false;
}
