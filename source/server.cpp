#include <QDebug>
#include <QtNetwork/QTcpServer>

#include "server.h"

Server::Server(QObject* parent)
	: QObject(parent) {
	tcpServer = new QTcpServer(this);
	connect(tcpServer, &QTcpServer::newConnection, this, &Server::handleClientConnection);
}

Server::~Server() {
	tcpServer->close();
}

void Server::startServer(int port) {
	if (!tcpServer->listen(QHostAddress::Any, port)) {
		qDebug() << "Server could not start!";
	}
	else {
		qDebug() << "Server started!";
	}
}

void Server::handleClientConnection() {
	currentSocket = tcpServer->nextPendingConnection();
	connect(currentSocket, &QTcpSocket::readyRead, this, &Server::readClientData);
	qDebug() << "Client connected!";
}

void Server::readClientData() {
	if (currentSocket) {
		QString message = QString::fromUtf8(currentSocket->readAll());
		qDebug() << "Message received: " << message;
		emit messageReceived(message);
	}
}

void Server::sendResponse(const QString& response) {
	if (currentSocket) {
		currentSocket->write(response.toUtf8());
		currentSocket->flush();
		currentSocket->waitForBytesWritten(3000);
		currentSocket->close();
		qDebug() << "Response sent: " << response;
	}
}