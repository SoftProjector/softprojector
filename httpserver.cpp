#include "httpserver.hpp"

#include <QDebug>

//using namespace std;

HttpServer::HttpServer(QObject *parent) : QObject(parent)
{
    isRunning = false;
}

HttpServer::~HttpServer()
{
    stopServer();
}

void HttpServer::startServer(quint16 port)
{
    server = new QTcpServer(this);

    // waiting for the web brower to make contact, this will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(startConnection()));

    if(!server->listen(QHostAddress::Any, port))
        isRunning = false;
    else
        isRunning = true;
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

    socket->write("HTTP/1.1 200 OK\r\n");       // \r needs to be before \n
    socket->write("Content-Type: text/xml\r\n");
    socket->write("Connection: close\r\n");     // Require two \r\n.
    socket->write("\r\n");

    socket->write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
    socket->write("<softprojector>\r\n");
    socket->write(xmlData.toUtf8());
    socket->write("</softprojector>\r\n");

    socket->disconnectFromHost();
}


void HttpServer::stopServer()
{
    server->close();
    delete server;
    isRunning = false;
}

void HttpServer::setBibleText(Verse bVerse, BibleSettings &bSets)
{
    // Translation flags
    bool havePrimary = ("none" != bSets.versions.primaryBible);
    bool haveSecondary = ("none" != bSets.versions.secondaryBible);
    bool haveTrinary = ("none" != bSets.versions.trinaryBible);

    xmlData = "";

    xmlData += "<bible class=\"primary\">\r\n";
    xmlData += "<verse>" + bVerse.primary_text + "</verse>\r\n";
    xmlData += "<reference>" + bVerse.primary_caption + "</reference>\r\n";
    xmlData += "<translation>" + bVerse.primary_abbr + "</translation>\r\n";
    xmlData += "</bible>\r\n";

    if (haveSecondary) {
        xmlData += "<bible class=\"secondary\">\r\n";
        xmlData += "<verse>" + bVerse.secondary_text + "</verse>\r\n";
        xmlData += "<reference>" + bVerse.secondary_caption + "</reference>\r\n";
        xmlData += "<translation>" + bVerse.secondary_abbr + "</translation>\r\n";
        xmlData += "</bible>\r\n";
    }

    if (haveTrinary) {
        xmlData += "<bible class=\"tertiary\">\r\n";
        xmlData += "<verse>" + bVerse.trinary_text + "</verse>\r\n";
        xmlData += "<reference>" + bVerse.trinary_caption + "</reference>\r\n";
        xmlData += "<translation>" + bVerse.trinary_abbr + "</translation>\r\n";
        xmlData += "</bible>\r\n";
    }
}

void HttpServer::setSongText(Stanza stanza, SongSettings &sSets)
{
    xmlData = "<song>\r\n";

    xmlData += "<stanza>" + stanza.stanza + "</stanza>\r\n";
    xmlData += "<title>" + stanza.stanzaTitle + "</title>\r\n";
    xmlData += "<tune>" + stanza.tune + "</tune>\r\n";
    xmlData += "<words_by>" + stanza.wordsBy + "</words_by>\r\n";
    xmlData += "<music_by>" + stanza.musicBy + "</music_by>\r\n";
    xmlData += "<number>" + QString::number(stanza.number) + "</number>\r\n";
    xmlData += "<is_last>" + QString::number(stanza.isLast) + "</is_last>\r\n";

    xmlData += "</song>\r\n";
}

void HttpServer::setAnnounceText(AnnounceSlide announce, TextSettings &aSets)
{
    xmlData = "<announcement>\r\n";
    xmlData += "<text>" + announce.text + "</text>\r\n";
    xmlData += "</announcement>\r\n";
}

void HttpServer::setBlank()
{
    xmlData = "";
}
