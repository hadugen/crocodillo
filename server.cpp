#include "server.h"

Server::Server(QObject *parent) : AbstractConnection(parent) {
    _serverSocket = new QTcpServer(this);
    connect(_serverSocket, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));
}

Server::~Server() {
    for(QTcpSocket *socket : _clients) {
        delete socket;
    }
    delete _serverSocket;
}

bool Server::start(QString ip, quint16 port) {
    Q_UNUSED(ip)
    Q_UNUSED(port)
    //return _serverSocket->listen(QHostAddress(ip), port);
    return _serverSocket->listen(QHostAddress::Any);
}

void Server::stop() {
    _serverSocket->close();
}

void Server::write(QByteArray message) {
    emit readyRead(message);
    for(QTcpSocket *client : _clients)
    {
        client->write(message);
    }
}

void Server::write(unsigned int clientNumber, QByteArray message) {
    if(clientNumber == _clients.size()) {
        emit readyRead(message);
    } else {
        _clients.at(clientNumber)->write(message);
    }
}

unsigned int Server::clientCount() {
    return _clients.size() + 1;
}

quint16 Server::port() const {
    return _serverSocket->serverPort();
}

void Server::slot_newConnection() {
    QTcpSocket *socket = _serverSocket->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(slot_clientDisconnected()));
    _clients.push_back(socket);
    emit clientConnected(socket->peerAddress().toString());
}

void Server::slot_readSocket() {
    QByteArray message = dynamic_cast<QTcpSocket*>(sender())->readAll();
    write(message);
}

void Server::slot_clientDisconnected() {
    QTcpSocket *socket = dynamic_cast<QTcpSocket*>(sender());
    emit clientDisconnected(socket->peerAddress().toString());
}
