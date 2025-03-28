#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client: public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);
    ~Client();

    Q_INVOKABLE void connectToServer(const QString &host, int port);
    void sendMessage(const QString &message);

signals:
    void startConnection();
    void connectSucessed(const QString);
    void connectError(QAbstractSocket::SocketError);
    void responseReceived(const QString &response);

public slots:
    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError);
    void readServerResponse();

private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_H
