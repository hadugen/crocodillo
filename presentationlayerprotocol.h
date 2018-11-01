#ifndef PRESENTATIONLAYERPROTOCOL_H
#define PRESENTATIONLAYERPROTOCOL_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <typeinfo>
#include <QTime>

struct Message
{
    QString definer, data;
    unsigned int size;
    char head = '{',
         tail = '}',
         argsSplitter = ',';

    operator QByteArray()
    {
        QByteArray msg;
        msg.push_back(head);
        msg.push_back(definer.toUtf8());
        msg.push_back(':');
        msg.push_back(data.toUtf8());
        msg.push_back(tail);
        size = msg.size();
        msg.push_front(QByteArray::number(size));
        return msg;
    }

};

class PresentationLayerProtocol : public QObject
{
    Q_OBJECT
public:
    static Message incapsulate(QString definer, const char *format, ...);
    static Message decapsulate(QByteArray message);
};

#endif // PRESENTATIONLAYERPROTOCOL_H
