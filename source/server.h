#pragma once
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

class Server :public QObject {
	Q_OBJECT

public:
	explicit Server(QObject* parent = nullptr);
	~Server();

	void startServer(int port);

signals:
	void messageReceived(const QString& message);
	void connected(const QString& s);

public slots:

	void handleClientConnection();
	void readClientData();
	void sendResponse(const QString& response);

private:
	QTcpServer* tcpServer;
	QTcpSocket* currentSocket;
};