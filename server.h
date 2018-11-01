#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include "abstractconnection.h"

class Server : public AbstractConnection
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    bool start(QString ip = QString(), quint16 port = 0);
    void stop();
    void write(QByteArray message);
    void write(unsigned int clientNumber, QByteArray message);
    unsigned int clientCount();
    quint16 port() const;

private:
    QList <QTcpSocket*> _clients;
    QTcpServer *_serverSocket;

signals:
    void clientConnected(QString hostAddress);
    void clientDisconnected(QString hostAddress);

private slots:
    void slot_newConnection();
    void slot_readSocket();
    void slot_clientDisconnected();
};

#endif // SERVER_H
