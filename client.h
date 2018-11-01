#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <abstractconnection.h>

class Client : public AbstractConnection
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
    ~Client();
    bool start(QString ip = QString(), quint16 port = 0);
    void stop();
    void write(QByteArray message);
    quint16 port() const;

private:
    QTcpSocket *_socket;

signals:
    void connectedToHost();

private slots:
    void slot_connected();
    void slot_readSocket();

};

#endif // CLIENT_H
