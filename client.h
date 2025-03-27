#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client: public QObject {
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);
    ~Client();

    bool connectToServer(const QString &host, int port);
    void sendMessage(const QString &message);

signals:
    void responseReceived(const QString &response);

public slots:
    void readServerResponse();

private:
    QTcpSocket *tcpSocket;
};

#endif // CLIENT_H

