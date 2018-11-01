#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <QObject>

class AbstractConnection : public QObject
{
    Q_OBJECT
public:
    explicit AbstractConnection(QObject *parent = 0);
    virtual bool start(QString ip = QString(), quint16 port = 0) = 0;
    virtual void stop() = 0;
    virtual void write(QByteArray message) = 0;
    virtual quint16 port() const = 0 ;

signals:
    void readyRead(QByteArray message);

private slots:
    virtual void slot_readSocket() = 0;
};

#endif // ABSTRACTCONNECTION_H
