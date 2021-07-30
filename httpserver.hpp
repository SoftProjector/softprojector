/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2021  Vladislav Kobzar
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

// Simple HTTP server implementation in Qt taken from https://stackoverflow.com/a/32931195

#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDebug>

#include "bible.hpp"
#include "song.hpp"
#include "announcement.hpp"

class HttpServer : public QObject
{
    Q_OBJECT

public:
    explicit HttpServer(QObject *parent = 0);
    ~HttpServer();
    void startServer(quint16 port);
    void stopServer();
    void setBibleText(Verse bVerse, BibleSettings &bSets);
    void setSongText(Stanza stanza, SongSettings &sSets);
    void setAnnounceText(AnnounceSlide announce, TextSettings &aSets);
    void setBlank();
    bool isRunning;
    //QTcpSocket *socket;

public slots:
    void startConnection();
    void sendData();
    void closingClient();

private:
    QTcpServer *server;
    qint64 bytesAvailable() const;
    QString xmlData;
    void establishConnection();
    bool servingConnection;

signals:
};

#endif
