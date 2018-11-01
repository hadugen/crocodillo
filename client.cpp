#include "client.h"

Client::Client(QObject *parent) : AbstractConnection(parent)
{
    _socket = new QTcpSocket(this);
    connect(_socket, SIGNAL(readyRead()), this, SLOT(slot_readSocket()));
    connect(_socket, SIGNAL(connected()), this, SLOT(slot_connected()));
}

Client::~Client()
{
    _socket->disconnectFromHost();
    delete _socket;
}

bool Client::start(QString ip, quint16 port)
{
    _socket->connectToHost(ip, port);
    qDebug() << _socket->errorString();
    return _socket->waitForConnected(500);
}

void Client::stop()
{
    _socket->close();
}

void Client::write(QByteArray message)
{
    _socket->write(message);
}

quint16 Client::port() const
{
   return _socket->peerPort();
}

void Client::slot_readSocket()
{
    qDebug() << "msg here";
    emit readyRead(_socket->readAll());
}

void Client::slot_connected()
{
    emit connectedToHost();
}


