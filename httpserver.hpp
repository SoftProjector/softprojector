// Simple HTTP server from https://stackoverflow.com/a/32931195

#ifndef HTTPSERVER
#define HTTPSERVER
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
