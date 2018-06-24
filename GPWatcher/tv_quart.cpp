#include "tv_quart.h"
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
TV_qUart::TV_qUart(QSerialPort *parent) : QSerialPort(parent)
{
    baudList << "115200"
             << "9600"
             << "19200"
             << "38400"
             << "57600"
             << "230400"
             << "1000000"
             << "2000000"
             << "4000000";
    parityList << "None parity"
               << "Even parity"
               << "Odd parity"
               << "Space parity"
               << "Mark parity" ;
    dataBitsList << "8 data bit"
                 << "7 data bit"
                 << "6 data bit"
                 << "5 data bit";
    stopBitsList << "1 Stop bit"
                 << "2 Stop bits"
                 << "1.5 Stop bits";

    connect(this, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(reportError(QSerialPort::SerialPortError)));
    connect(this, SIGNAL(readyRead()),this,SLOT(handleData()));
}



int TV_qUart::currentPendingRaws(){
    return pendingRawData.count();
}


//轉換後分派資料的位置，在這裡處理期待可以分擔一些 main thread 的負擔。
void TV_qUart::handleData(){
    TV_qUart::PENDING_RAW_DATA_TypeDef data;
    QDateTime dt;
    data.time = dt.currentMSecsSinceEpoch();
    dt.setMSecsSinceEpoch(data.time);
    data.data = readAll();
    data.isRX = true;

    QString xx = "TX";
    if(data.isRX) xx = "RX";
    QString prefix = QString("%1 ::  ").arg(dt.toString(QString("hh:mm:ss.zzz %1:").arg(xx)));
    data.string_orig = QString(data.data);
    data.string_hex = prefix;
    for(int i=0;i<data.data.count();i++){
        data.string_hex += QString("%1 ").arg((unsigned char)data.data.at(i),2,16,QChar('0')).toUpper();
    }
    data.string_hex += "\n\n";
    pendingRawData.append(data);
    while(pendingRawData.count()>keepRaw){ pendingRawData.removeFirst(); }
    //處理資料
    emit serialInfoIn();
}


TV_qUart::PENDING_RAW_DATA_TypeDef TV_qUart::popPendingInfo(){
    if  (pendingRawData.isEmpty()){
        TV_qUart::PENDING_RAW_DATA_TypeDef d;
        d.data = QByteArray();
        d.time = 0;
        return d;
    }
    TV_qUart::PENDING_RAW_DATA_TypeDef pop = pendingRawData.at(0);
    pendingRawData.removeFirst();
    return pop;
}

void TV_qUart::reportError(QSerialPort::SerialPortError error){
    if (error == QSerialPort::ResourceError) {
        emit serialConnectionEvent(UART_DISCONNECT_DUE_TO_RESOURCE_ERROR);
        this->close();
    }
}

void TV_qUart::connectDialog(bool toggled){
    if(!toggled){
        this->close();
        emit serialConnectionEvent(UART_DISCONNECTED_MANUALLY);
        return;
    }

    QDialog dialog;
    QFormLayout form(&dialog);
    form.setSpacing(1);
    QComboBox *portBox = new QComboBox(&dialog);
    QStringList newList;
    newList.append(usedPortName);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
        if (!newList.contains(info.portName())){
            newList.append(info.portName());
        }
    }
    portBox->addItems(newList);
    QComboBox *baudBox = new QComboBox();
    baudBox->addItems((const QStringList)baudList);
    baudBox->hide();

    QComboBox *parityBox = new QComboBox(&dialog);
    parityBox->addItems(parityList);
    parityBox->hide();

    QComboBox *dataBitBox = new QComboBox(&dialog);
    dataBitBox->addItems(dataBitsList);
    dataBitBox->hide();

    QComboBox *stopBitBox = new QComboBox(&dialog);
    stopBitBox->addItems(stopBitsList);
    stopBitBox->hide();


    QPushButton *more = new QPushButton(&dialog);
    connect(more, SIGNAL(clicked()),baudBox,SLOT(show()));
    connect(more, SIGNAL(clicked()),parityBox,SLOT(show()));
    connect(more, SIGNAL(clicked()),dataBitBox,SLOT(show()));
    connect(more, SIGNAL(clicked()),stopBitBox,SLOT(show()));
    connect(more, SIGNAL(clicked()),more,SLOT(hide()));
    more->setText("more...");
    more->setFlat(true);


    form.addRow(new QLabel("Insert relative info."));
    form.addRow(portBox);
    form.addRow(more);
    form.addRow(baudBox);
    form.addRow(parityBox);
    form.addRow(dataBitBox);
    form.addRow(stopBitBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);
    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));
    if (dialog.exec() == QDialog::Accepted) {
        for(int i=usedPortName.count()-1;i>=0;i--){
            if (usedPortName.at(i) == portBox->currentText()){
                usedPortName.removeAt(i);
            }
        }
        usedPortName.push_front(portBox->currentText());

        for(int i=baudList.count()-1;i>=0;i--){
            if (baudList.at(i) == baudBox->currentText()){
                baudList.removeAt(i);
            }
        }
        baudList.push_front(baudBox->currentText());

        for(int i=parityList.count()-1;i>=0;i--){
            if (parityList.at(i) == parityBox->currentText()){
                parityList.removeAt(i);
            }
        }
        parityList.push_front(parityBox->currentText());

        for(int i=dataBitsList.count()-1;i>=0;i--){
            if (dataBitsList.at(i) == dataBitBox->currentText()){
                dataBitsList.removeAt(i);
            }
        }
        dataBitsList.push_front(dataBitBox->currentText());

        for(int i=stopBitsList.count()-1;i>=0;i--){
            if (stopBitsList.at(i) == stopBitBox->currentText()){
                stopBitsList.removeAt(i);
            }
        }
        stopBitsList.push_front(stopBitBox->currentText());
        openSerialPort(portBox->currentText(), baudBox->currentText(), parityBox->currentText(), dataBitBox->currentText(),stopBitBox->currentText());
    }else{
        emit serialConnectionEvent(UART_CONNECT_CANCEL);
    }
}

void TV_qUart::serialDataOut(QByteArray tx_data)
{
    qDebug() << "serialDataOut in";
    this->write(tx_data);
    TV_qUart::PENDING_RAW_DATA_TypeDef data;
    QDateTime dt;
    data.time = dt.currentMSecsSinceEpoch();
    dt.setMSecsSinceEpoch(data.time);
    data.data = tx_data;
    data.isRX = false;

    QString xx = "TX";
    if(data.isRX) xx = "RX";
    QString prefix = QString("%1 ::  ").arg(dt.toString(QString("hh:mm:ss.zzz %1:").arg(xx)));
    data.string_orig = QString(data.data);
    data.string_hex = prefix;
    for(int i=0;i<data.data.count();i++){
        data.string_hex += QString("%1 ").arg((unsigned char)data.data.at(i),2,16,QChar('0')).toUpper();
    }
    data.string_hex += "\n\n";
    pendingRawData.append(data);
    while(pendingRawData.count()>keepRaw){ pendingRawData.removeFirst(); }
    //處理資料
    emit serialInfoIn();
}




void TV_qUart::openSerialPort(QString portname, QVariant baud, QVariant parity, QVariant dataBit, QVariant stopBit){
    this->close();
    this->setPortName(portname);
    this->setBaudRate(baud.toInt());
    QString stopBitStr = stopBit.toString();
    if(stopBitStr == "1 Stop bit"){
        this->setStopBits(QSerialPort::OneStop       );
    }else if(stopBitStr == "2 Stop bits"){
        this->setStopBits(QSerialPort::TwoStop       );
    }else if(stopBitStr == "1.5 Stop bits"){
        this->setStopBits(QSerialPort::OneAndHalfStop);
    }else{
        this->setStopBits(QSerialPort::OneStop       );
        stopBitStr = "1 Stop bit";
    }


    QString dataBitStr = dataBit.toString();
    if(dataBitStr == "5 data bit"){
        this->setDataBits(QSerialPort::Data5       );
    }else if(dataBitStr == "6 data bit"){
        this->setDataBits(QSerialPort::Data5       );
    }else if(dataBitStr == "7 data bit"){
        this->setDataBits(QSerialPort::Data5);
    }else if(dataBitStr == "8 data bit"){
        this->setDataBits(QSerialPort::Data8);
    }else{
        this->setDataBits(QSerialPort::Data8       );
        dataBitStr = "8 data bit";
    }

    QString parityStr = parity.toString();
    if(parityStr == "None parity"){
        this->setParity(QSerialPort::NoParity       );
    }else if(parityStr == "Even parity"){
        this->setParity(QSerialPort::EvenParity       );
    }else if(parityStr == "Odd parity"){
        this->setParity(QSerialPort::OddParity);
    }else if(parityStr == "Space parity"){
        this->setParity(QSerialPort::SpaceParity);
    }else if(parityStr == "Mark parity"){
        this->setParity(QSerialPort::MarkParity);
    }else{
        this->setParity(QSerialPort::NoParity       );
        parityStr = "None parity";
    }

    this->setFlowControl(QSerialPort::NoFlowControl);
    if (this->open(QIODevice::ReadWrite)) {
        emit serialConnectionEvent(UART_CONNECT_SUCCESS);
    } else {
        this->close();
        emit serialConnectionEvent(UART_CONNECT_FAIL);
    }
}

