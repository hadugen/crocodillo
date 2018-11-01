#include "presentationlayerprotocol.h"

Message PresentationLayerProtocol::incapsulate(QString definer, const char *format, ...)
{
    const char **argAddr = &format + 1;
    QStringList argTypes = QString(format).split('%', QString::SkipEmptyParts);
    Message msg;
    msg.definer = definer;
    for(int i = 0; i < argTypes.size(); i++, argAddr++)
    {
        QString argType = argTypes[i];

        if(i != 0)
            msg.data.push_back(msg.argsSplitter);

        if(argType == "c")
        {
            char c = (char&)*argAddr;
            msg.data.push_back(c);
            continue;
        }

        if(argType == "cp")
        {
            char *cp = (char*)*argAddr;
            msg.data.push_back(cp);
            continue;
        }

        if(argType == "i")
        {
            int i = (int&)*argAddr;
            msg.data.push_back(QByteArray::number(i));
            continue;
        }

        if(argType == "ui")
        {
            unsigned int ui = (unsigned int&)*argAddr;
            msg.data.push_back(QByteArray::number(ui));
            continue;
        }

        if(argType == "d")
        {
            double d = (double&)*argAddr;
            argAddr++;
            msg.data.push_back(QByteArray::number(d, 'f'));
            continue;
        }

    }
    return msg;
}

Message PresentationLayerProtocol::decapsulate(QByteArray message)
{
    Message msg;
    unsigned int definerEndsAt = message.indexOf(':'),
                 definerBeginsAt = message.indexOf(msg.head),
                 dataEndsAt = message.lastIndexOf(msg.tail);

    msg.definer = message.mid(definerBeginsAt + 1, definerEndsAt - definerBeginsAt - 1);
    msg.data = message.mid(definerEndsAt + 1, dataEndsAt - definerEndsAt - 1);
    return msg;
}
