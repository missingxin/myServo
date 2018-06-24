#ifndef TV_QUART_H
#define TV_QUART_H

#include <QObject>
#include <QWidget>
#include <QtGui>
#include <QtCore>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFormLayout>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QLabel>
#include <QDialog>
#include <QStringList>
#include <QString>
#include <QList>
class TV_qUart : public QSerialPort
{
    Q_OBJECT
public:
    explicit TV_qUart(QSerialPort *parent = 0);
    void connectDialog(bool toggled);
    typedef enum{
        UART_CONNECT_SUCCESS        = 0,
        UART_CONNECT_FAIL           ,
        UART_CONNECT_CANCEL         ,
        UART_CONNECT_ERROR_VALUE    ,
        UART_DISCONNECTED           ,
        UART_DISCONNECTED_MANUALLY  ,
        UART_DISCONNECT_DUE_TO_RESOURCE_ERROR,
    }TV_UART_CONNECT_EVENT_TypeDef;

    typedef struct{
        qint64 time;
        QByteArray data;
        bool isRX;
        QString string_hex;
        QString string_orig;
    }PENDING_RAW_DATA_TypeDef;


    PENDING_RAW_DATA_TypeDef popPendingInfo();
    int currentPendingRaws();
    void keepRawData(int howManyDataYouNeedToKeep){ keepRaw = howManyDataYouNeedToKeep; }


signals:
    void serialConnectionEvent(TV_qUart::TV_UART_CONNECT_EVENT_TypeDef);
    void serialDataIn(QByteArray);
    void serialInfoIn();

private slots:
    void handleData();
    void reportError(QSerialPort::SerialPortError error);

public slots:
    void serialDataOut(QByteArray tx_data);

private:

    void openSerialPort(QString portname, QVariant baud, QVariant parity, QVariant dataBit, QVariant stopBit);
    QStringList usedPortName;
    QStringList baudList;
    QStringList parityList;
    QStringList dataBitsList;
    QStringList stopBitsList;
    QList<PENDING_RAW_DATA_TypeDef>pendingRawData;
    bool remove_ascii_space  = false; //移除空白
    bool remove_ascii_newLine  = false; // 移除結尾的\r\n
    int keepRaw = 0xFFFF; //保留的raw資料
};

#endif // TV_QUART_H
