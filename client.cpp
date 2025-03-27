#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) :
    QObject(parent), tcpSocket(new QTcpSocket(this)) {
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readServerResponse);
}

Client::~Client() {
    tcpSocket->close();
}

bool Client::connectToServer(const QString &host, int port) {
    tcpSocket->connectToHost(host, port);
    if(!tcpSocket->waitForConnected(3000)) {
        qDebug() << "Could not connect to server";
        return false;
    } else {
        qDebug() << "Connected to server";
        return true;
    }
}

void Client::sendMessage(const QString &message) {
    tcpSocket->write(message.toUtf8());
    tcpSocket->flush();
    tcpSocket->waitForBytesWritten(3000);
    qDebug() << "Message sent";
}

void Client::readServerResponse() {
    QString response = QString::fromUtf8(tcpSocket->readAll());
    qDebug() << "Response received:" << response;
    emit responseReceived(response);
}
