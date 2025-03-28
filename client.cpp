#include "client.h"
#include <QDebug>

Client::Client(QObject *parent) :
    QObject(parent), tcpSocket(new QTcpSocket(this)) {

    connect(tcpSocket, &QTcpSocket::connected, this, &Client::onSocketConnected);
    connect(tcpSocket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &Client::onSocketError);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Client::readServerResponse);
}

Client::~Client() {
    tcpSocket->close();
}

void Client::connectToServer(const QString &host, int port) {
    tcpSocket->connectToHost(host, port);
    emit startConnection();
}

void Client::sendMessage(const QString &message) {
    tcpSocket->write(message.toUtf8());
    tcpSocket->flush();
    tcpSocket->waitForBytesWritten(3000);
    qDebug() << "Message sent";
}

void Client::onSocketConnected() {
    emit connectSucessed("?");
}
void Client::onSocketError(QAbstractSocket::SocketError error) {
    emit connectError(error);
}

void Client::readServerResponse() {
    QString response = QString::fromUtf8(tcpSocket->readAll());
    qDebug() << "Response received:" << response;
    emit responseReceived(response);
}
