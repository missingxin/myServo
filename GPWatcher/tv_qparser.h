#ifndef TV_QPARSER_H
#define TV_QPARSER_H

#include <QObject>
#include <QWidget>

class TV_qParser : public QObject
{
    Q_OBJECT
public:
    explicit TV_qParser(QObject *parent = 0);

    virtual void parser(QByteArray) = 0;
    typedef struct{
        qint64 time;
        bool isRX;
        QByteArray packet_data;
    }PENDING_PACKET_DATA_TypeDef;

    void setExpireTime(int ms){ expire_time = ms;}
signals:

public slots:

private:
    int expire_time = 100;
    QList<QByteArray> pendingPackets;
};

#endif // TV_QPARSER_H
